#ifndef ReviewCamera_h
#define ReviewCamera_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

enum PressKey{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0;
const float PITCH = 0.0; 
const float SENSITIVITY = 0.1;
const float SPEED = 2.5;
const float ZOOM = 45.0;

class Camera{
public:
    glm::vec3 Up;
    glm::vec3 worldUp;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Position;
    
    float Yaw;
    float Pitch;
    float mouseSensitivity;
    float movementSpeed;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 worldup = glm::vec3(0.0, 1.0, 0.0),  float yaw = YAW, float pitch = PITCH) :
    Front(glm::vec3(0.0, 0.0, -1.0)), mouseSensitivity(SENSITIVITY), movementSpeed(SPEED), Zoom(ZOOM){
        Position = position;
        worldUp = worldup;
        Yaw = yaw;
        Pitch = Pitch;
        updateCameraVertex();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    Front(glm::vec3(0.0, 0.0, -1.0)), mouseSensitivity(SENSITIVITY), movementSpeed(SPEED), Zoom(ZOOM){
        Position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVertex();
    }
    
    glm::mat4 getViewMatrix(){
        return glm::lookAt(Position, Position + Front, Up);
    }
    
    void processKeyboard(PressKey presskey, float deltaTime){
        float velocity = deltaTime * movementSpeed;
        if(presskey == FORWARD)
            Position += Front * velocity;
        if(presskey == BACKWARD)
            Position -= Front * velocity;
        if(presskey == LEFT)
            Position -= Right * velocity;
        if(presskey == RIGHT)
            Position += Right * velocity;
        
    }
    
    void processMouseMovement(float xoffset, float yoffset, bool constrain){
        Yaw += xoffset * mouseSensitivity;
        Pitch += yoffset * mouseSensitivity;
        
        if(constrain){
            if(Pitch >= 89.0)
                Pitch = 89.0;
            if(Pitch <= -89.0)
                Pitch = -89.0;
        }
        updateCameraVertex();
    }
    
    void processMouseScroll(float yoffset){
        if(Zoom >= 1.0 && Zoom <= 45.0)
            Zoom -= yoffset;
        if(Zoom >= 45.0)
            Zoom = 45.0;
        if(Zoom <= 1.0)
            Zoom = 1.0;
    }
    
    
private:
    void updateCameraVertex(){
        glm::vec3 front;
        front.x = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
        front.y = glm::sin(glm::radians(Pitch));
        front.z = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, worldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
