/**
 * Copyright 2010 Riku Palomäki.
 * This file is part of GLSL Lab.
 *
 * GLSL Lab is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLSL Lab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLSL Lab.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "shader/uniform.hpp"

#include <QtOpenGL>

#include <stdexcept>

/**
 * Uniform variable type info class, makes easier to implement more generic
 * code that does something with uniform variables.
 */
struct ShaderTypeInfo {
  typedef void (*FloatSetter)(GLint, GLsizei, const GLfloat*);
  typedef void (*IntSetter)(GLint, GLsizei, const GLint*);
  typedef void (*MatrixSetter)(GLint, GLsizei, GLboolean, const GLfloat*);

  /**
   * Constructor that also stores all the type information of given OpenGL type.
   *
   * @param info Bit field of type information:
   *             0x01 bit on == float type
   *             0x02 bit on == matrix type
   */
  ShaderTypeInfo(int size = 0, int info = 0, FloatSetter float_setter = 0,
      IntSetter int_setter = 0, MatrixSetter matrix_setter = 0, GLenum type = 0);

  /// Data type of the uniform variable, for example GL_FLOAT_MAT2x3
  /// - see the whole list from glGetActiveUniform documentation.
  GLenum type;
  /// Number of elements in type, Different from 1 only for vector and matrix types.
  int size;

  /// Is this a type where one element is a float
  bool is_float;
  /// Is this a type where one element is a integer
  bool is_int;
  /// Is this a matrix type
  bool is_matrix;

  /// For float types, this is the correct setter function, like glUniform3fv.
  FloatSetter float_setter;
  /// For integer types, this is the correct setter function, like glUniform3iv.
  IntSetter int_setter;
  /// For matrix types, this is the correct setter function, like glUniformMatrix2x3fv.
  MatrixSetter matrix_setter;

  /// Return a TypeInfo object for given type
  /// @param type Data type of the uniform variable, for example GL_FLOAT_MAT2x3 - 
  //              see the whole list from glGetActiveUniform documentation
  static const ShaderTypeInfo& typeInfo(GLenum type);
};

ShaderTypeInfo::ShaderTypeInfo(int size_, int info, FloatSetter float_setter_,
    IntSetter int_setter_, MatrixSetter matrix_setter_, GLenum type_)
  : type(type_), size(size_),
    is_float(info & 0x01), is_int(!is_float), is_matrix(info & 0x02),
    float_setter(float_setter_), int_setter(int_setter_), matrix_setter(matrix_setter_) {}

const ShaderTypeInfo& ShaderTypeInfo::typeInfo(GLenum type) {
  static QMap<GLenum, ShaderTypeInfo> types;
  typedef ShaderTypeInfo P;

  if (types.empty()) {
    // Add all known uniform types to the list
    types[GL_FLOAT]             = P(1, 1, glUniform1fv, 0, 0, GL_FLOAT);
    types[GL_FLOAT_VEC2]        = P(2, 1, glUniform2fv, 0, 0, GL_FLOAT_VEC2);
    types[GL_FLOAT_VEC3]        = P(3, 1, glUniform3fv, 0, 0, GL_FLOAT_VEC3);
    types[GL_FLOAT_VEC4]        = P(4, 1, glUniform4fv, 0, 0, GL_FLOAT_VEC4);
    types[GL_INT]               = P(1, 0, 0, glUniform1iv, 0, GL_INT);
    types[GL_INT_VEC2]          = P(2, 0, 0, glUniform2iv, 0, GL_INT_VEC2);
    types[GL_INT_VEC3]          = P(3, 0, 0, glUniform3iv, 0, GL_INT_VEC3);
    types[GL_INT_VEC4]          = P(4, 0, 0, glUniform4iv, 0, GL_INT_VEC4);
    types[GL_BOOL]              = P(1, 0, 0, glUniform1iv, 0, GL_BOOL);
    types[GL_BOOL_VEC2]         = P(2, 0, 0, glUniform2iv, 0, GL_BOOL_VEC2);
    types[GL_BOOL_VEC3]         = P(3, 0, 0, glUniform3iv, 0, GL_BOOL_VEC3);
    types[GL_BOOL_VEC4]         = P(4, 0, 0, glUniform4iv, 0, GL_BOOL_VEC4);
    types[GL_FLOAT_MAT2]        = P(4, 3, 0, 0, glUniformMatrix2fv, GL_FLOAT_MAT2);
    types[GL_FLOAT_MAT3]        = P(9, 3, 0, 0, glUniformMatrix3fv, GL_FLOAT_MAT3);
    types[GL_FLOAT_MAT4]        = P(16, 3, 0, 0, glUniformMatrix4fv, GL_FLOAT_MAT4);
    types[GL_FLOAT_MAT2x3]      = P(6, 3, 0, 0, glUniformMatrix2x3fv, GL_FLOAT_MAT2x3);
    types[GL_FLOAT_MAT2x4]      = P(8, 3, 0, 0, glUniformMatrix2x4fv, GL_FLOAT_MAT2x4);
    types[GL_FLOAT_MAT3x2]      = P(6, 3, 0, 0, glUniformMatrix3x2fv, GL_FLOAT_MAT3x2);
    types[GL_FLOAT_MAT3x4]      = P(12, 3, 0, 0, glUniformMatrix3x4fv, GL_FLOAT_MAT3x4);
    types[GL_FLOAT_MAT4x2]      = P(8, 3, 0, 0, glUniformMatrix4x2fv, GL_FLOAT_MAT4x2);
    types[GL_FLOAT_MAT4x3]      = P(12, 3, 0, 0, glUniformMatrix4x3fv, GL_FLOAT_MAT4x3);
    types[GL_SAMPLER_1D]        = P(1, 0, 0, glUniform1iv, 0, GL_SAMPLER_1D);
    types[GL_SAMPLER_2D]        = P(1, 0, 0, glUniform1iv, 0, GL_SAMPLER_2D);
    types[GL_SAMPLER_3D]        = P(1, 0, 0, glUniform1iv, 0, GL_SAMPLER_3D);
    types[GL_SAMPLER_CUBE]      = P(1, 0, 0, glUniform1iv, 0, GL_SAMPLER_CUBE);
    types[GL_SAMPLER_1D_SHADOW] = P(1, 0, 0, glUniform1iv, 0, GL_SAMPLER_1D_SHADOW);
    types[GL_SAMPLER_2D_SHADOW] = P(1, 0, 0, glUniform1iv, 0, GL_SAMPLER_2D_SHADOW);

    // error value
    types[GL_FALSE]             = P(0, 0, 0, 0, 0, 0);
  }

  QMap<GLenum, ShaderTypeInfo>::const_iterator it = types.find(type);
  if (it != types.end()) return *it;
  return types[GL_FALSE];
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

UniformVar::UniformVar(QGLShaderProgram& prog, QString name, GLenum type)
    : m_name(name), m_type(type), m_size(1) {
  const ShaderTypeInfo& info = ShaderTypeInfo::typeInfo(type);
  if (info.size <= 0) throw std::runtime_error("Unknown Uniform variable type");

  m_location.push_back(glGetUniformLocation(prog.programId(), name.toStdString().c_str()));

  /// @todo Do we know if the array locations are in adjacent locations?
  ///       location[n] seems to be always location[0] + n, and glUniform*v
  ///       -functions allow changing the whole array with only the location
  ///       of the first element.
  for (int i = 1;; ++i) {
    GLint l = glGetUniformLocation(prog.programId(),
        (name + '[' + QString::number(i) + ']').toStdString().c_str());
    if (l >= 0) {
      m_location.push_back(l);
      ++m_size;
    } else break; // the end of the array, or not an array at all
  }

  if (info.is_float) {
    // For example vec3[6] needs 6*3 floats
    m_floatdata.resize(m_size * info.size);
    for (size_t i = 0; i < m_size; ++i) {
      glGetUniformfv(prog.programId(), m_location[i], &m_floatdata.front() + i * info.size);
    }
  } else {
    m_intdata.resize(m_size * info.size);
    for (size_t i = 0; i < m_size; i++) {
      glGetUniformiv(prog.programId(), m_location[i], &m_intdata.front() + i * info.size);
    }
  }
}

void UniformVar::set(QGLShaderProgram& prog, bool relocate) {
  const ShaderTypeInfo& info = ShaderTypeInfo::typeInfo(m_type);

  /// @todo should we set every array element one by one?
  ///       Things wouldn't break so easy if the array size changes.
  /*
  for (int i = 0; i < m_size; ++i) {
    GLint loc = m_location[i];
    if (relocate) loc = glGetUniformLocation(prog.programId(), (i == 0 ? m_name :
        m_name + '[' + QString::number(i) + ']').toStdString().c_str());

    if (loc == -1) continue;
  }
  */

  GLint loc = m_location[0];
  if (relocate) loc = glGetUniformLocation(prog.programId(), m_name.toStdString().c_str());

  if (loc == -1) return;

  if (info.is_matrix) {
    info.matrix_setter(loc, m_size, 0, &m_floatdata.front());
  } else if (info.is_float) {
    assert(!m_floatdata.empty());
    info.float_setter(loc, m_size, &m_floatdata.front());
  } else {
    assert(!m_intdata.empty());
    info.int_setter(loc, m_size, &m_intdata.front());
  }
}

QVariant UniformVar::getValue(QVariant::Type t, int idx) {
  const ShaderTypeInfo& info = ShaderTypeInfo::typeInfo(m_type);

  QVariant qv;
  if (info.size == 1) {
    qv = QVariant(t);
    if (info.is_float) {
      qv.setValue(m_floatdata[idx]);
    } else {
      qv.setValue(m_intdata[idx]);
    }
  } else {
    QString tmp;
    for (int i = 0; i < info.size; i++) {
      if (i != 0) tmp += ", ";
      tmp += info.is_float ? QString::number(m_floatdata[info.size*idx+i])
        : QString::number(m_intdata[info.size*idx+i]);
    }
    qv = QVariant(tmp);
  }
  return qv;
}
