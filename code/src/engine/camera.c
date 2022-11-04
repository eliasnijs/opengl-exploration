/* function implementations */
internal void
camera_update_vectors(Camera *camera)
{
  vec3 front = {
    cos(deg_to_rad(camera->yaw))*cos(deg_to_rad(camera->pitch)),
    sin(deg_to_rad(camera->pitch)),
    sin(deg_to_rad(camera->yaw))*cos(deg_to_rad(camera->pitch)),
  };
  vec3_norm(camera->front, front);

  vec3_mul_cross(camera->right, camera->front, camera->world_up);
  vec3_norm(camera->right, camera->right);

  vec3_mul_cross(camera->up, camera->right, camera->front);
  vec3_norm(camera->up, camera->up);
}

internal void
camera_init(Camera *camera, vec3 pos, vec3 up, real32 yaw, real32 pitch,
            real32 speed, real32 mouse_sensitivity,
            bool32 constrain_pitch, real32 zoom)
{
  vec3 front = {0.0f,0.0f,-1.0f};
  memcpy(camera->front, front, sizeof(vec3));
  memcpy(camera->pos, pos, sizeof(vec3));
  memcpy(camera->world_up, up, sizeof(vec3));
  camera->yaw = yaw;
  camera->pitch = pitch;
  camera_update_vectors(camera);

  camera->speed = speed;
  camera->mouse_sensitivity = mouse_sensitivity;
  camera->constrain_pitch = constrain_pitch;
  camera->zoom = zoom;
}

internal void
camera_get_M_view(Camera *camera, mat4x4 M_view)
{
  vec3 camera_target;
  vec3_add(camera_target, camera->pos, camera->front);
  mat4x4_look_at(M_view, camera->pos, camera_target, camera->up);
}

internal void
camera_handle_keyboard_input(Camera *camera, Camera_Dir dir, real32 dt)
{
  real32 v = camera->speed*dt;
  vec3 vtemp;
  if (dir == FORWARD)
  {
    vec3_scale(vtemp, camera->front, v);
    vec3_add(camera->pos, camera->pos, vtemp);
  }
  if (dir == BACKWARD)
  {
    vec3_scale(vtemp, camera->front, v);
    vec3_sub(camera->pos, camera->pos, vtemp);
  }
  if (dir == LEFT)
  {
    vec3_scale(vtemp, camera->right, v);
    vec3_sub(camera->pos, camera->pos, vtemp);
  }
  if (dir == RIGHT)
  {
    vec3_scale(vtemp, camera->right, v);
    vec3_add(camera->pos, camera->pos, vtemp);
  }
}

internal void
camera_handle_mouse_movement_input(Camera *camera, real32 xoffset, real32 yoffset)
{
  camera->yaw   += xoffset*camera->mouse_sensitivity;
  camera->pitch += yoffset*camera->mouse_sensitivity;
  if (camera->constrain_pitch)
    camera->pitch = Clamp(-PITCH_CLAMP, camera->pitch, PITCH_CLAMP);
  camera_update_vectors(camera);
}

internal void
camera_handle_mouse_zoom_input(Camera *camera, real32 yoffset)
{
  camera->zoom = Clamp(ZOOM_CLAMP_LB, camera->zoom - yoffset, ZOOM_CLAMP_UB);
}























