// Project Phase 2
// Teresa Hume
// tch562
// 10152929

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// simple class to hold Model Textures
namespace ModelParser
{
    class Texture
    {
        protected string texture;

        public Texture(string _texture)
        {
            texture = _texture;
        }

        public string TextureFile
        {
            get { return texture; }
        }

    }
}
