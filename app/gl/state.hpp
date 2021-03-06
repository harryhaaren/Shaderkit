/**
 * Copyright 2010-2012 Riku Palomäki.
 * This file is part of Shaderkit, http://www.shaderkit.org/.
 *
 * Shaderkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation.
 *
 * Shaderkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shaderkit.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STATE_HPP
#define STATE_HPP

#include "gl/opengl.hpp"
#include "gl/buffer_object.hpp"

#include <QSet>
#include <QMap>
#include <QPoint>

namespace Shaderkit
{

  /**
   * Represents OpenGL state.
   *
   * This class is meant to work as a replacement for plain OpenGL 2 API. It is
   * faster to keep track of all server-side GL capabilities and only
   * enable/disable minimal set of those when needed. Also some things that were
   * just some magical built-in global variables in shaders are replaced in
   * newer OpenGL versions for example with custom uniform variables, and setting
   * those correctly is a job for this class.
   *
   * Currently the apply() method doesn't actually do anything, and for example
   * all glEnable/glDisable calls are made directly in the enable/disable methods.
   *
   * @todo Actually store the OpenGL state and do those things documented.
   */
  class State
  {
  public:
    State(Scene& scene, float time, float dt);

    /// Returns the next available light id, can be used like GL_LIGHT0 + id
    int nextFreeLight() const;
    /// Reserve/Release a light id
    void setLight(int light_id, Light* light);

    /// Set GL capability
    void enable(GLenum cap);
    /// Get GL capability
    void disable(GLenum cap);

    /// Reserves and returns next free texture unit
    int reserveTexUnit(void* keyptr, QString keyname);

    /// Saves the state
    void push();
    /// Restores the saved state
    void pop();

    void pushMaterial(MaterialPtr);
    void popMaterial();

    /// Multiplies the current matrix from right
    void pushModel(const glm::mat4& model);
    void popModel();

    /// Return model matrix
    const glm::mat4& model() const;

    /// Vertex attributes of the currently bind program
    VertexAttrib& attr();

    /// Matrix from model coordinates to window coordinates
    /// @param swap_y enable if you want to project to Qt coordinates
    glm::mat4 transform(bool swap_y = false) const;

    void setSelection(QList<ObjectPtr> objects);
    QList<ObjectPtr> selection() const;

    MaterialPtr material() const;
    QSet<MaterialPtr> usedMaterials() const { return m_usedMaterials; }

    ProgramPtr prog() const;
    void useProgram(ProgramPtr prog);

    void setCamera(CameraPtr camera);
    CameraPtr camera() const;

    float time() const { return m_time; }
    float dt() const { return m_dt; }

    void setPicking(QPoint pos);
    QPoint pickingPos() const;
    bool picking() const;
    void setPicked(ObjectPtr o, MeshPtr m);
    QPair<ObjectPtr, MeshPtr> picked();
    void disablePicking();

    unsigned int pickingQuery();

    void applyUniformsMappings();
    void setUniform(GLProgram& prog, const QString& name, const glm::mat4& m);

  protected:
    struct Data {
      QMap<QPair<void*, QString>, int> m_texunits;
      QMap<int, Light*> m_lights;
      CameraPtr m_camera;
      ProgramPtr m_prog;
      VertexAttrib m_attr;
    };

    Scene& m_scene;

    QList<Data> m_data;
    QList<MaterialPtr> m_materials;
    QSet<MaterialPtr> m_usedMaterials;
    QList<ObjectPtr> m_selection;
    float m_time, m_dt;

    QPoint m_picking_point;
    bool m_picking;
    QPair<ObjectPtr, MeshPtr> m_picked;

    std::vector<glm::mat4> m_transforms;

    int nextFree(const QSet<int>& lst, int id = 0) const;
  };

} // namespace Shaderkit

#endif // STATE_HPP
