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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "forward.hpp"
#include "gl/scene_object.hpp"

#include "core/attribute.hpp"

#include "glm/glm.hpp"

#include <QVariant>

#ifdef _MSC_VER
#undef near
#undef far
#endif

namespace Shaderkit
{
  /**
   * OpenGL Camera class, handles perspective and ortho projections.
   */
  class Camera : public QObject, public std::enable_shared_from_this<Camera>,
      public SceneObject
  {
    Q_OBJECT

  public:
    enum Type { Perspective, Ortho, Rect };

    Camera(const QString& name);

    /// Set the camera with screen size width x height
    void prepare(int width, int height);

    QVariantMap toMap() const;

    /// Load the camera from map
    void load(QVariantMap map);

    CameraPtr clone() const;

    QIcon icon();

    float width() const { return m_width; }
    float height() const { return m_height; }

    /// Sets this camera to use orthographic matrix that fills the screen
    void setRect(float near = -1.0f, float far = 1.0f);

    void setNear(float near);
    float near() const { return m_near.value(); }

    void setFar(float far);
    float far() const { return m_far.value(); }

    void setFov(float fov);
    float fov() const { return m_fov.value(); }

    void rotate(const glm::vec2& diff);
    void translate(const glm::vec2& diff);
    void zoom(float diff);

    void setTarget(const glm::vec3& target);
    const glm::vec3& target() const;

    void setLocation(const glm::vec3& location);
    const glm::vec3 location() const;

    const glm::vec3 up() const;
    const glm::vec3 right() const;
    const glm::vec3 front() const;

    Type type() const { return m_type.value(); }
    void setType(Type type);

    float dist() const;

    //void setPickDisplay(float x, float y);

    const glm::mat4& projection() const { return m_projection; }

    const glm::mat4& view() const { return m_view; }

    /// @param swap_y enable if you want to project to Qt coordinates
    glm::mat4 normToWindow(bool swap_y = false) const;

    /// Matrix from view coordinates to window coordinates
    /// @param swap_y enable if you want to project to Qt coordinates
    glm::mat4 transform(bool swap_y = false) const;

    virtual void attributeChanged();

  signals:
    void changed(CameraPtr);

  protected:
    explicit Camera(const Camera& c);
    void updateVectors();

  protected:
    Attribute<Type> m_type;

    /// The point the camera is looking at (Perspective)
    Attribute<glm::vec3> m_target;

    /// Camera up vector (Perspective)
    /// There are cached values
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_front;

    Attribute<float> m_dx, m_dy, m_dist;

    glm::mat4 m_projection;
    glm::mat4 m_view;

    float m_width, m_height;

    /// Field of view, in degrees, in the y direction (Perspective)
    Attribute<float> m_fov;
    /// Near clipping plane distance
    Attribute<float> m_near;
    /// Far clipping plane distance
    Attribute<float> m_far;
  };

} // namespace Shaderkit

#endif // CAMERA_HPP
