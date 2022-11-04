typedef struct shader Shader;
struct shader
{
  uint32 id;
};

internal uint32 shader_init(const char *vertex_path, const char *fragment_path, Shader *shader);
internal uint32 shader_init_module(const char *path, int32 type, uint32 *module_id);
internal void shader_die(Shader *shader);
internal void shader_use(Shader *shader);
internal void shader_set_bool32(Shader *shader, char *name, bool32 value);
internal void shader_set_int32(Shader *shader, char *name, int32 value);
internal void shader_set_real32(Shader *shader, char *name, real32 value);
internal void shader_set_mat4x4(Shader *shader, char *name, mat4x4 m);
