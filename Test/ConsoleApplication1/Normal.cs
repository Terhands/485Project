// Project Phase 2
// Teresa Hume
// tch562
// 10152929

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// simple class to store model surface normal vectors
namespace ModelParser
{
    class Normal
    {
        private double x, y, z;

        public Normal(double _x, double _y, double _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        public double X
        {
            get { return x; }
        }

        public double Y
        {
            get { return y;  }
        }

        public double Z
        {
            get { return z; }
        }
    }
}
