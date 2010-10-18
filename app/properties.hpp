/**
 * Copyright 2010 Riku Palomäki.
 * This file is part of GLSL Lab.
 *
 * GLSL Lab is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation.
 *
 * GLSL Lab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLSL Lab.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include "forward.hpp"
#include "shader/uniform.hpp"

#include <QMap>

class Properties : public QTreeWidget {
  Q_OBJECT

public:
  Properties(QWidget* parent = 0);
  virtual ~Properties() {}

protected:
};

/**
 * Real-time property editor for shader properties, like uniform variables.
 *
 * This is a singleton class.
 */
class ShaderProperties : public Properties {
  Q_OBJECT

public:
  static ShaderProperties& instance();
  ShaderProperties(QWidget* parent = 0);
  virtual ~ShaderProperties();

public slots:
  /// This shader program has changed (usually just relinked)
  void update(ProgramPtr shader);
  /// Remove a shader from the property list
  void remove(ProgramPtr shader);
  /// User changed property value (to variant)
  //void valueChanged(QtProperty* property, const QVariant& variant);

protected:
/*  typedef QMap<QtProperty*, UniformVar> PropertyMap;

  /// Maps the property to uniform variable
  PropertyMap m_properties;

  /// Every shader has one group property whose children are the actual uniform variables
  QMap<ProgramPtr, QtVariantProperty*> m_shaders;*/

  static ShaderProperties* s_instance;
};

/**
 * List of all available editable files.
 *
 * This is a singleton class.
 */
class FileList : public QTreeWidget {
  Q_OBJECT

public:
  static FileList& instance();
  FileList(QWidget* parent = 0);
  virtual ~FileList();

signals:
  void openFile(ShaderPtr);

public slots:
  void update(ShaderPtr shader);
  void activateFile(QTreeWidgetItem* item, int column);
  void remove(ShaderPtr shader);

protected:
  QMap<QString, QTreeWidgetItem*> m_files;
  QMap<QTreeWidgetItem*, ShaderPtr> m_items;
  QTreeWidgetItem* m_src;

  static FileList* s_instance;
};

#endif // PROPERTIES_HPP
