
// Project Phase 2
// Teresa Hume
// tch562
// 10152929

// Initial code taken from A1 sample solution & extended to support textures & surface normals
// C# program to reformat the model file

//A class that hodls triangle vertex indices data

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// simple class to store Model Triangles
namespace ModelParser
{
    class Triangle
    {
        // the vertices of this triangle
        private int vertice1;
        private int vertice2;
        private int vertice3;

        // this triangle's surface normal
        private int normalIndex;

        // the texture to map onto this triangle
        private int textureIndex;

        // vertex 1's texture map coordinate
        private double v1tx;
        private double v1ty;

        // vertex 2's texture map coordinate
        private double v2tx;
        private double v2ty;

        // vertex 3's texture map coordinate
        private double v3tx;
        private double v3ty;

        public Triangle(int _vertice1, int _vertice2, int _vertice3, int _normalIndex, int _textureIndex, 
                        double _v1tx, double _v1ty, double _v2tx, double _v2ty, double _v3tx, double _v3ty)
        {
            vertice1 = _vertice1;
            vertice2 = _vertice2;
            vertice3 = _vertice3;

            normalIndex = _normalIndex;
            textureIndex = _textureIndex;

            v1tx = _v1tx;
            v1ty = _v1ty;

            v2tx = _v2tx;
            v2ty = _v2ty;

            v3tx = _v3tx;
            v3ty = _v3ty;

        }

        public int Vertice1
        {
            get { return vertice1; }
        }

        public int Vertice2
        {
            get { return vertice2; }
        }


        public int Vertice3
        {
            get { return vertice3; }
        }

        public int Normal
        {
            get { return normalIndex; }
        }

        public int Texture
        {
            get { return textureIndex; }
        }

        public double V1tx
        {
            get { return v1tx; }
        }

        public double V1ty
        {
            get { return v1ty; }
        }

        public double V2tx
        {
            get { return v2tx; }
        }

        public double V2ty
        {
            get { return v2ty; }
        }
        public double V3tx
        {
            get { return v3tx; }
        }

        public double V3ty
        {
            get { return v3ty; }
        }

    }


}
