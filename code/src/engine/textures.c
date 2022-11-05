internal uint32
texture_load(const char *path)
{
  uint32 texture_id = 0;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  int32 width, height, nr_of_channels;
  uint8 *data = stbi_load(path, &width, &height, &nr_of_channels, 0);
  if (data)
  {
    GLenum format;
    switch (nr_of_channels)
    {
      case (1): format = GL_RED;
                break;
      case (3): format = GL_RGB;
                break;
      case (4): format = GL_RGBA;
                break;
      default: DebugLogError("nr of channels of texture %s is not supported", path);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
                 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else
  {
    texture_id = 0;
    DebugLogError("failed to load texture %s", path);
  }

  stbi_image_free(data);
  return texture_id;
}
