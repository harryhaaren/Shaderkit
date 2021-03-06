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

#ifndef RENDER_PASS_PROPERTIES_HPP
#define RENDER_PASS_PROPERTIES_HPP

#include "gui/properties.hpp"

#include <QComboBox>
#include <QPushButton>

namespace Shaderkit
{

  /*class TextureEditor : public QWidget {
    Q_OBJECT

  public:
    TextureEditor(RenderPassPtr pass);

    QLineEdit* target() { return m_target; }

    void setTex(TexturePtr tex);
    TexturePtr tex() { return m_tex; }

  protected slots:
    void listActivated(int index);
    void updateList();
    void newClicked();
    void editClicked();

  protected:
    RenderPassPtr m_pass;

    TexturePtr m_tex;
    QComboBox* m_list;
    QListWidget* m_view;
    QLineEdit* m_target;
  };

  class TexturesEditor : public QObject, public QTreeWidgetItem {
    Q_OBJECT

  public:
    TexturesEditor(QTreeWidgetItem* parent, RenderPassPtr pass);

  public slots:
    void updated(RenderPassPtr);

  protected:
    RenderPassPtr m_pass;
    QList<QPair<QTreeWidgetItem*, TextureEditor*>> m_editors;
  };*/

  struct PassItem {
    HeaderWidget* header;
    QPersistentModelIndex header_index;

    QComboBox* material_box;
    QComboBox* view_box;

    QLineEdit* size;
    QPushButton* size_btn;

    QPushButton* clear_color;
    QPushButton* clear_depth;
    QPushButton* clear_stencil;

    QLabel* objects_label;
    QLabel* lights_label;
  };

  /**
   * Real-time property editor for render passes.
   *
   * This is a singleton class.
   */
  class RenderPassProperties : public QTableWidget
  {
    Q_OBJECT

  public:
    static RenderPassProperties& instance();
    RenderPassProperties(QWidget* parent = 0);
    virtual ~RenderPassProperties();

    void init();

  protected:
    /// It seems that there is some bug in Qt4 (or maybe we are using it somehow wrong).
    /// If we render this thing normally having QHeaderView::ResizeToContents,
    /// ScrollPerPixel, setStretchLastSection and ScrollBarAsNeeded and we are actuall
    /// hidden (in QTabWidget), before we render the widget the first time, Qt toggles
    /// vertical header on and off every frame, causing massive resize wave eating all
    /// CPU time. So we are hacking around that by setting last of those styles only
    /// after first render.
    void paintEvent(QPaintEvent* e);

  public slots:
    void changed(RenderPassPtr pass);
    void listUpdated(QList<RenderPassPtr> passes);
    void selectionChanged();

    void create();
    void reorder();
    void duplicate();
    void remove();

    void materialListUpdated();
    void cameraListUpdated();

  private slots:
    void openObjectBrowser();
    void openLightEditor();
    void openCameraEditor();

    void sizeEdited();
    void toggleAutoSize();

    void clearChanged();
    void openClearColorPicker();
    void setClearColor(QColor);

    void materialActivated(int);
    void cameraActivated(int);

  protected:
    //void dropEvent(QDropEvent* event);
    //virtual void contextMenuEvent(QContextMenuEvent* e);

    RenderPassPtr get(QModelIndex index) const;
    RenderPassPtr getSelected() const;
    RenderPassPtr getSender() const;

    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex& index,
        const QEvent* event) const;

    QWidget* insertItem(int row, const QString& name, PropertyLayout*& pl);
    QPushButton* createButton(RenderPassPtr pass, QWidget* container,
                              const QIcon& icon, const char* method);

    QMap<RenderPassPtr, PassItem> m_passes;
    QMap<QObject*, RenderPassPtr> m_senders;

    std::shared_ptr<PropertyLayoutData> m_data;

    ScenePtr m_scene;
    RenderPassPtr m_selected;
    RenderPassPtr m_pass;

    QAction* m_create, *m_reorder, *m_duplicate, *m_destroy;
    QList<RenderPassPtr> m_list;

    /// See paintEvent for explanation for this
    bool m_firstRender;

    static RenderPassProperties* s_instance;
  };

} // namespace Shaderkit

#endif // RENDER_PASS_PROPERTIES_HPP
