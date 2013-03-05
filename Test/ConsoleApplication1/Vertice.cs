
// Project Phase 2
// Teresa Hume
// tch562
// 10152929

// Initial code taken from A1 sample solution & extended to support textures & surface normals
// C# program to reformat the model file

//A class that holds vertex coordinate data
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelParser
{
    public class Vertice
    {

        private double x, y, z;

        public double X
        {
            get { return x; }
        }

        public double Y
        {
            get { return y; }
        }


        public double Z
        {
            get { return z; }
        }

        public Vertice(double x, double y, double z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

    }

}
