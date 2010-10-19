#ifndef RENDER_PASS_PROPERTIES_HPP
#define RENDER_PASS_PROPERTIES_HPP

#include "properties.hpp"

class ShaderEditor : public QWidget {
  Q_OBJECT

public:
  ShaderEditor(RenderPassPtr pass);

public slots:
  void updateShaderList();

protected slots:
  void listActivated(int index);

protected:
  RenderPassPtr m_pass;
  QComboBox* m_shaderlist;
};

class SizeEditor : public QWidget {
  Q_OBJECT

public:
  SizeEditor(RenderPassPtr pass);

public slots:
  void updateSize(RenderPassPtr);

protected slots:
  void sizeChanged();
  void btnToggled(bool);

protected:
  RenderPassPtr m_pass;
  QLineEdit* m_size;
  QPushButton* m_autobtn;
};

class ObjectInserter : public QWidget {
  Q_OBJECT

public:
  ObjectInserter(RenderPassPtr pass);

public slots:
  void updateObjectList();

protected slots:
  void listActivated(int index);
  void updated(RenderPassPtr);

protected:
  RenderPassPtr m_pass;
  QComboBox* m_availableObjects;
};

class ObjectEditor : public QWidget {
  Q_OBJECT

public:
  ObjectEditor(RenderPassPtr pass, ObjectPtr obj);

protected slots:
  void editClicked();
  void deleteClicked();

protected:
  RenderPassPtr m_pass;
  ObjectPtr m_obj;
};

class ObjectsEditor : public QObject, public QTreeWidgetItem {
  Q_OBJECT

public:
  ObjectsEditor(QTreeWidgetItem* parent, RenderPassPtr pass);

public slots:
  void updated(RenderPassPtr);

protected:
  RenderPassPtr m_pass;
  QMap<ObjectPtr, QPair<QTreeWidgetItem*, ObjectEditor*> > m_objs;
};

/**
 * Real-time property editor for render passes.
 *
 * This is a singleton class.
 */
class RenderPassProperties : public Properties {
  Q_OBJECT

public:
  static RenderPassProperties& instance();
  RenderPassProperties(QWidget* parent = 0);
  virtual ~RenderPassProperties();

public slots:
  /// This render pass has been changed / created
  void update(RenderPassPtr pass);
  /// Removes the render pass
  void remove(RenderPassPtr pass);
  /// User changed property value (to variant)
  //void valueChanged(QtProperty* property, const QVariant& variant);

protected:
  struct Sub {
    QTreeWidgetItem* item;

    Sub() : item(0) {}
  };

  void init(Sub& sub, RenderPassPtr pass);

  /// Every render pass has one group property whose children are the actual passes
  QMap<RenderPassPtr, Sub> m_renderpasses;
  /*
  struct Sub {
    enum Type {
      Clear
    };

    Sub() : obj(0) {}
    QtVariantProperty* obj;
    QtVariantProperty* clear;
  };


  /// Every render pass has one group property whose children are the actual passes
  QMap<RenderPassPtr, Sub> m_renderpasses;

  typedef QMap<QtProperty*, QPair<Sub::Type, RenderPassPtr> > PropertyMap;

  PropertyMap m_properties;*/

  static RenderPassProperties* s_instance;
};


#endif // RENDER_PASS_PROPERTIES_HPP
