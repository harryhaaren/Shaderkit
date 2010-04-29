#include "scene.hpp"
#include "state.hpp"
#include "renderpass.hpp"
#include "object3d.hpp"
#include "json_value.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "shader/program.hpp"

#include <QtOpenGL>

/// @todo move to Object3D
static ObjectPtr createObject(const QString& name) {
  if (name == "teapot") {
    return ObjectPtr(new Teapot);
  } else if (name == "box") {
    return ObjectPtr(new Box);
  }
  return ObjectPtr();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Scene::Scene() : m_width(-1), m_height(-1) {}

void Scene::resize(int width, int height) {
  if (width == m_width && height == m_height) return;

  m_width = width;
  m_height = height;
}

void Scene::render() {
  State state;
  for (RenderPasses::iterator it = m_render_passes.begin(); it != m_render_passes.end(); ++it) {
    (*it)->render(state);
  }
}

void Scene::load(const Value& value) {
  Value::Map vm = value.map("objects");
  for (Value::Map::iterator it = vm.begin(); it != vm.end(); ++it) {
    ObjectPtr object;
    QString type = it->second.str("model.0");
    QString name = it->second.str("model.1");
    if (type == "built-in") {
      object = createObject(name);
    } else if (type == "file") {
      //object = loadModel(name);
    }

    if (object) m_objects[it->first] = object;
  }

  vm = value.map("lights");
  for (Value::Map::iterator it = vm.begin(); it != vm.end(); ++it) {
    LightPtr light(new Light(it->first));
    light->load(it->second);
    m_lights[it->first] = light;
  }


  vm = value.map("cameras");
  for (Value::Map::iterator it = vm.begin(); it != vm.end(); ++it) {
    CameraPtr camera(new Camera(it->first));
    camera->load(it->second);
    m_cameras[it->first] = camera;
  }

  vm = value.map("shaders");
  for (Value::Map::iterator it = vm.begin(); it != vm.end(); ++it) {
    ProgramPtr shader(new GLProgram(it->first));
    Value& v = it->second;
    /// @todo change the format so that there can be many shaders of same type
    if (v.have("fragment")) shader->addShader(v.str("fragment"), QGLShader::Fragment);
    if (v.have("vertex")) shader->addShader(v.str("vertex"), QGLShader::Vertex);
    if (v.have("geometry")) shader->addShader(v.str("geometry"), QGLShader::Geometry);
    m_shaders[it->first] = shader;
  }

  vm = value.map("render passes");
  for (Value::Map::iterator it = vm.begin(); it != vm.end(); ++it) {
    RenderPassPtr pass(new RenderPass(shared_from_this()));
    pass->load(it->second);
    m_render_passes.push_back(pass);
  }
}