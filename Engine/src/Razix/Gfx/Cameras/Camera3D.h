#pragma once

// Std. Includes
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

namespace Razix {
    namespace Gfx {
        // Default camera values
        const f32 YAW        = -90.0f;
        const f32 PITCH      = 0.0f;
        const f32 SPEED      = 0.025f;
        const f32 SENSITIVTY = 0.25f;
        const f32 ZOOM       = 45.0f;

        // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
        enum Camera_Movement_Direction
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN
        };

        class RAZIX_API Camera3D
        {
        public:
            // Constructor with vectors
            Camera3D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), f32 yaw = YAW, f32 pitch = PITCH);
            // Constructor with scalar values
            Camera3D(f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, f32 yaw, f32 pitch);
            // Update the camera movement in the world space
            void update(d32 deltaTime);
            // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
            void processKeyboard(Camera_Movement_Direction direction, d32 deltaTime);
            // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
            void processMouseMovement(f32 xoffset, f32 yoffset, bool constrainPitch = true);
            // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
            void processMouseScroll(f32 yoffset);

            RAZIX_INLINE const glm::vec3& getPosition() const { return Position; }
            void                          setPosition(glm::vec3 vector)
            {
                Position       = vector;
                TargetMovement = Position;
                updateCameraVectors();
            }

            RAZIX_INLINE const glm::vec3& getForward() const { return Front; }
            void                          setForward(glm::vec3 vector)
            {
                Front = vector;
                updateCameraVectors();
            }

            RAZIX_INLINE const glm::vec3& getUp() const { return Up; }
            void                          setUp(glm::vec3 vector)
            {
                Up = vector;
                updateCameraVectors();
            }

            RAZIX_INLINE const glm::vec3& getRight() const { return Right; }
            void                          setRight(glm::vec3 vector)
            {
                Right = vector;
                updateCameraVectors();
            }

            RAZIX_INLINE const glm::vec3& getWorldUp() const { return WorldUp; }
            void                          setWorldUp(glm::vec3 vector)
            {
                WorldUp = vector;
                updateCameraVectors();
            }

            RAZIX_INLINE const f32& getYaw() const { return Yaw; }
            void                    setYaw(f32 value)
            {
                Yaw = value;
                updateCameraVectors();
            }

            RAZIX_INLINE const f32& getPitch() const { return Pitch; }
            void                    setPitch(f32 value)
            {
                Pitch = value;
                updateCameraVectors();
            }

            RAZIX_INLINE const f32& getZoom() const { return Zoom; }
            void                    setZoom(f32 value)
            {
                Zoom = value;
                updateCameraVectors();
            }

            RAZIX_INLINE const f32& getSpeed() const { return MovementSpeed; }
            void                    setSpeed(f32 speed)
            {
                MovementSpeed = speed;
                updateCameraVectors();
            }

            RAZIX_INLINE const f32& getSensitivity() const { return MouseSensitivity; }
            void                    setSensitivity(f32 sensitivity)
            {
                MouseSensitivity = sensitivity;
                updateCameraVectors();
            }

            // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
            RAZIX_FORCE_INLINE glm::mat4 getViewMatrix();

            glm::mat4 getViewMatrixLH();
            glm::mat4 getViewMatrixRH();

        private:
            // Camera Attributes
            glm::vec3 Position;
            glm::vec3 Front = {-0.1, -0.18, -0.88};
            glm::vec3 Up;
            glm::vec3 Right;
            glm::vec3 WorldUp;
            // Euler Angles
            f32 Yaw   = YAW;
            f32 Pitch = PITCH;
            // Camera options
            f32 MovementSpeed    = SPEED;
            f32 MouseSensitivity = SENSITIVTY;
            f32 Zoom             = ZOOM;

            f32 m_OldX = 0, m_OldY = 0;

            glm::vec3 TargetMovement = glm::vec3(0.0f);
            glm::vec3 Velocity       = glm::vec3(0.0f);    // Stores the current movement velocity
            float     DampingFactor  = 0.85f;              // Controls how quickly movement slows down

        private:
            // Calculates the front vector from the Camera's (updated) Euler Angles
            void updateCameraVectors();
        };

    }    // namespace Gfx
}    // namespace Razix
