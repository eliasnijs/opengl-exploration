typedef struct Vertex Vertex;
struct Vertex
{
  vec3 Position;
  vec3 Normal;
  vec2 TexCoords;
};

typedef struct Texture Texture;
struct Texture
{
  uint32 id;
  char *type;
};

typedef struct Mesh
struct Mesh
{
  Vertex  *vertices;
  uint32  len_vertices;

  uint32  *indices;
  uint32  len_indices;

  Texture *textures;
  uint32  len_textures;

  uint32 VAO;
  uint32 VBO;
  uint32 EBO;
};

internal void
mesh_init(Vertex *vertices, uint32 len_vertices,
          uint32 *indices, uint32 len_indices,
          Texture *textures, uint32 len_textures,
          Mesh *mesh)
{
  mesh->vertices = vertices;
  mesh->indices  = indices;
  mesh->textures = textures;
  mesh_setup(mesh);
}

internal void
mesh_setup(Mesh *mesh)
{
  glGenArrays(1, &mesh->VAO);
  glGenBuffers(1, &mesh->VBO);
  glGenBuffers(1, &mesh->EBO);

  glBindVertexArray(mesh->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

  glBufferData(GL_ARRAY_BUFFER, mesh->len_vertices * sizeof(Vertex),
               mesh->vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)OffsetOf(Vertex, Normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)OffsetOf(Vertex, TexCoords));

  glBindVertexArray(0);
}

