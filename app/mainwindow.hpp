/**
 * Copyright 2010,2011 Riku Palomäki.
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "forward.hpp"
#include "shader/error.hpp"
#include "watcher.hpp"
#include "opengl.hpp"

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QPushButton>

#include <QtGui/QDialog>
#include <QGLWidget>

class QGLContext;

/// Clickable custom button used in the editor status bar inside the tab widget.
/// @todo this is used in render_pass_properties, move to somewhere else
class IconBtn : public QPushButton {
  Q_OBJECT

public:
  IconBtn(QWidget* parent);
  virtual ~IconBtn();

  QSize sizeHint() const;
  void setPadding(QSize p) { m_padding = p; }

public slots:
  /// Custom rendering of the widget, because it is the only way to make sure
  /// the system doesn't draw some weird lines around the button when it's active.
  virtual void paintEvent(QPaintEvent*);

private:
  QSize m_padding;
};

/**
 * All other widgets are part of the widget tree that the Main Window starts as a root widget.
 */
class MainWindow : public QMainWindow, public Watchable {
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

  /// Creates a new editor widget for given material, and adds the editor to a new tab.
  MultiEditor* createEditor(MaterialPtr material);

  void activateEditor(MultiEditor* editor);

//  QList<MultiEditor*> editors() { return m_editors; }

  static MainWindow& instance();
  static ScenePtr scene();

  template <typename T>
  T* createGL(QWidget* parent) {
    T* t = 0;
    if (m_context)
      t = new T(m_context, parent);

    if (!t) {
      foreach (QGLWidget* w, m_glwidgets) {
        if (w->isValid()) {
          t = new T(formatGL(), parent, w);
          break;
        }
      }
    }

    if (!t && !m_glwidgets.isEmpty())
      t = new T(formatGL(), parent, m_glwidgets.first());

    if (!t)
      t = new T(formatGL(), parent);

    m_glwidgets << t;
    m_context = 0;
    return t;
  }

  void destroyedGL(QGLWidget* widget);

  QGLFormat formatGL() const;

public slots:
  /// Updates the error list
  void updateErrors(ShaderErrorList errors);
  void about();

  bool openScene(ScenePtr scene);
  bool openScene(QString filename);
  /// Load a new project from file, open a file browser
  bool load();
  /// Reloads the project file, basically just loads the same file again,
  /// but tries to keep the same state
  bool reload();
  void setSceneChanged(bool status);

  QList<GLSLEditor*> findEditors(ShaderPtr shader);
  QList<GLSLEditor*> findEditors(QString res);
  MultiEditor* findEditor(MaterialPtr mat);
  MultiEditor* openMaterial(MaterialPtr mat);

  /// A file was changed on the disk.
  /// Delegates handling the event to right editor, or if no editor is open
  /// with this file, forces the shader to reload the file.
  void fileUpdated(const QString& filename);

  /// An editors modification status is changed and the star after the tab name is added/removed.
  void modificationChanged(MultiEditor* editor, bool b);

  /// Does the editor ask for autocompiling the shader after codeChanged().
  bool autoCompileEnabled() const;

  static void cleanup();

signals:
  void newScene(ScenePtr);

protected:
  void keyPressEvent(QKeyEvent* event);
  void changeEvent(QEvent* e);
  void restore();
  bool closeScene();

  void createViewport();

protected slots:
  /// Focus the correct tab and line for selected error.
  void errorItemActivated(QTableWidgetItem*);
  /// Save the current file (the file open in the active editor)
  void saveMaterial(int index = -1);
  /// Save the current project
  void saveScene();
  void closeEditor(int index);

  void closeEvent(QCloseEvent* event);
  void changed();
  void setSandboxCompiler(bool);

  void import();
  void compileAll();

  void openTextureBrowser();

  void setAutosaveScene(bool);
  void saved();

private:
  /// Main layout generated from the .ui -file.
  std::auto_ptr<Ui::MainWindow> m_ui;

  /// Currently there is exactly one scene for one MainWindow.
  ScenePtr m_scene;

  /// List of all editors and their respective root widgets in m_ui->editor_tabs.
  QMap<MultiEditor*, QWidget*> m_editors;

  /// Maps one item in the error_list (column 0) to correct error
  QMap<QTableWidgetItem*, ShaderError> m_error_list_items;

  QList<QGLWidget*> m_glwidgets;
  QGLContext* m_context;

  QAction* m_sync;

  bool m_sceneChanged;
  static MainWindow * s_instance;
};

class About : public QDialog
{
  Q_OBJECT
public:
  About(QWidget* parent);
};

#endif // MAINWINDOW_HPP
