/* constants */
global_variable const real32 YAW_STD          =  45.0f;
global_variable const real32 PITCH_STD        =   0.0f;
global_variable const real32 SPEED_STD        =   2.5f;
global_variable const real32 SENSITIVITY_STD  =   0.1f;
global_variable const real32 ZOOM_STD         =  45.0f;
global_variable const real32 PITCH_CLAMP      =  89.0f;
global_variable const real32 ZOOM_CLAMP_LB    =   1.0f;
global_variable const real32 ZOOM_CLAMP_UB    =  45.0f;

/* enumerations */
typedef enum camera_dir Camera_Dir;
enum camera_dir
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

typedef struct camera Camera;
struct camera
{
  vec3 pos;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 world_up;

  real32 yaw;
  real32 pitch;

  real32 speed;
  real32 mouse_sensitivity;
  bool32 constrain_pitch;
  real32 zoom;
};

/* function declarations */
internal void camera_update_vectors(Camera *camera);
internal void camera_init(Camera *camera, vec3 pos, vec3 up, real32 yaw, real32 pitch,
                          real32 speed, real32 mouse_sensitivity, bool32 constrain_pitch, real32 zoom);
internal void camera_get_M_view(Camera *camera, mat4x4 M_view);
internal void camera_handle_keyboard_input(Camera *camera, Camera_Dir dir, real32 dt);
internal void camera_handle_mouse_movement_input(Camera *camera, real32 xoffset, real32 yoffset);
internal void camera_handle_mouse_zoom_input(Camera *camera, real32 yoffset);
