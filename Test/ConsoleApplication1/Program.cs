
// Project Phase 2
// Teresa Hume
// tch562
// 10152929

// Initial code taken from A1 sample solution & extended to support textures & surface normals
// C# program to reformat the model file

//INTRODUCTION: This is the C# file that handles the parsing of the model file 
//and writes a simplified version of the file that is easier to read into C++
//as C# can handle the parsing in less lines than C++
//This class uses Triangle.cs and Vertice.cs to hold triangle vertex indices and 
//vertex data respectively that is contained in the model file. Some basic error 
//handling is included in c#

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;



namespace ModelParser
{
    class Program
    {
        static void Main(string[] args)
        {
            //The model file text is read into one large string
            Console.WriteLine(args[0]);
            string model_file = System.IO.File.ReadAllText(args[0]);

            //Use a regex match with the proper regex string + capture group to capture the model name within the XML tags
            Match name_match = Regex.Match(model_file, @"<name>(?<model_name>[A-Za-z_0-9\s]*)</name>");
            string model_name = name_match.Groups[1].Value.Trim();

            Console.WriteLine(model_name);

            //Here I am extracting the model vertices contents within the vertices XML tags
            string model_vertices = "";
            //Use the following regex expressiona nd capture group to match the blob of text containing the vertices,
            //then tokenize them and store the coordinate data into an array of object Vertice
            Match vertices_match = Regex.Match(model_file, @"<.*vertices.*>(?<model_vertices>[.\s\w\d-]*)<.*/vertices.*>");
            model_vertices = vertices_match.Groups[1].Value.Trim();
            model_vertices = Regex.Replace(model_vertices, @"\n", "");
            model_vertices = Regex.Replace(model_vertices, @"[\s\t]{1,}", " ");
            string[] tokenized_vertice_coordinates = model_vertices.Split(' ');
            int num_vertices = tokenized_vertice_coordinates.Length / 3;


            Vertice[] vertices_array = new Vertice[num_vertices];
            for (int i = 0, j = 0; i < tokenized_vertice_coordinates.Length; i += 3, j += 1)
            {

                vertices_array[j] = new Vertice(
                                                   Convert.ToDouble(tokenized_vertice_coordinates[i]),
                                                   Convert.ToDouble(tokenized_vertice_coordinates[i + 1]),
                                                   Convert.ToDouble(tokenized_vertice_coordinates[i + 2]));
            }





            //Like was done with the vertices, do the same to the triangle vertex indices data
            string model_triangles = "";
            Match triangles_match = Regex.Match(model_file, @"<.*triangles.*>(?<triangles>[.\s\w\d-]*)<.*/triangles.*>");
            model_triangles = triangles_match.Groups[1].Value.Trim();
            model_triangles = Regex.Replace(model_triangles, @"\n", "");
            model_triangles = Regex.Replace(model_triangles, @"[\s\t]{1,}", " ");
            string[] tokenized_triangle_match = model_triangles.Split(' ');
            int num_triangles = tokenized_triangle_match.Length / 11;
            Triangle[] triangles_array = new Triangle[num_triangles];

            for (int i = 0, j = 0; i < tokenized_triangle_match.Length; i += 11, j += 1)
            {
                triangles_array[j] = new Triangle(
                                                 Convert.ToInt32(tokenized_triangle_match[i]),
                                                 Convert.ToInt32(tokenized_triangle_match[i + 1]),
                                                 Convert.ToInt32(tokenized_triangle_match[i + 2]),
                                                 Convert.ToInt32(tokenized_triangle_match[i + 3]),
                                                 Convert.ToInt32(tokenized_triangle_match[i + 4]),
                                                 Convert.ToDouble(tokenized_triangle_match[i + 5]),
                                                 Convert.ToDouble(tokenized_triangle_match[i + 6]),
                                                 Convert.ToDouble(tokenized_triangle_match[i + 7]),
                                                 Convert.ToDouble(tokenized_triangle_match[i + 8]),
                                                 Convert.ToDouble(tokenized_triangle_match[i + 9]),
                                                 Convert.ToDouble(tokenized_triangle_match[i + 10])
                                                 );
            }

            string model_normals = "";
            Match normals_match = Regex.Match(model_file, @"<.*normals.*>(?<normals>[.\s\w\d-]*)<.*/normals.*>");
            model_normals = normals_match.Groups[1].Value.Trim();
            model_normals = Regex.Replace(model_normals, @"\n", "");
            model_normals = Regex.Replace(model_normals, @"[\s\t]{1,}", " ");
            string[] tokenized_normal_match = model_normals.Split(' ');
            int num_normals = tokenized_normal_match.Length / 3;
            Normal[] normals_array = new Normal[num_normals];
            
            for (int i = 0, j = 0; i < tokenized_normal_match.Length; i += 3, j += 1)
            {
                normals_array[j] = new Normal(
                                             Convert.ToDouble(tokenized_normal_match[i]),
                                             Convert.ToDouble(tokenized_normal_match[i + 1]),
                                             Convert.ToDouble(tokenized_normal_match[i + 2])
                                             );
            }
            
            string model_textures = "";
            Match textures_match = Regex.Match(model_file, @"<.*textures.*>(?<textures>[.\s\w\d-]*)<.*/textures.*>");
            model_textures = textures_match.Groups[1].Value.Trim();             // strip out leading & trailing whitespace
            model_textures = Regex.Replace(model_textures, @"\n", "");          // strip out newlines
            model_textures = Regex.Replace(model_textures, @"[\s\t]{1,}", " ");  // each texture is separated by a single space
            string[] tokenized_texture_match = model_textures.Split(' ');       // split the textures up by the delimiting space
            int num_textures = tokenized_texture_match.Length;                  // track how many textures were pulled out of the model file
            Texture[] textures_array = new Texture[num_textures];
            
            // populate the textures array
            for (int i = 0; i < tokenized_texture_match.Length; i += 1)
            {
                textures_array[i] = new Texture(tokenized_texture_match[i]);
            }
            

            //At this point, all the relevant data for phase1 has been captured. Now write the data, in a simplified
            //format, line, by line into a file called model.txt, which will be used by the renderer program, phase1.exe
            //to read the model data and render it
            System.IO.File.Delete("C:\\Users\\Terhands\\Desktop\\Test\\ConsoleApplication1\\ModelResources\\model.txt");
            System.IO.StreamWriter file = new System.IO.StreamWriter("C:\\Users\\Terhands\\Desktop\\Test\\ConsoleApplication1\\ModelResources\\model.txt", true);

            file.WriteLine(model_name);

            file.WriteLine(num_vertices);
            foreach (string s in tokenized_vertice_coordinates)
            {
                file.WriteLine(s);
            }

            file.WriteLine(num_triangles);
            foreach (Triangle t in triangles_array)
            {
                file.WriteLine(t.Vertice1);
                file.WriteLine(t.Vertice2);
                file.WriteLine(t.Vertice3);
                file.WriteLine(t.Normal);
                file.WriteLine(t.Texture);
                file.WriteLine(t.V1tx);
                file.WriteLine(t.V1ty);
                file.WriteLine(t.V2tx);
                file.WriteLine(t.V2ty);
                file.WriteLine(t.V3tx);
                file.WriteLine(t.V3ty);
            }

            file.WriteLine(num_normals);
            foreach (Normal n in normals_array)
            {
                file.WriteLine(n.X);
                file.WriteLine(n.Y);
                file.WriteLine(n.Z);
            }

            
            file.WriteLine(num_textures);
            foreach (Texture t in textures_array)
            {
                file.WriteLine(t.TextureFile);
            }
            
            //close the file
            file.Close();




            //Writing human readable parse results to a separate file for interpretation and
            //proof that the my model parser is parsing the models correctly
            System.IO.File.Delete("C:\\Users\\Terhands\\Desktop\\Test\\ConsoleApplication1\\ModelResources\\model file parse results.txt");
            System.IO.StreamWriter file_parse_results = new System.IO.StreamWriter("C:\\Users\\Terhands\\Desktop\\Test\\ConsoleApplication1\\ModelResources\\model file parse results.txt");
            file_parse_results.WriteLine("Model name: " + model_name);
            file_parse_results.WriteLine("Number of vertices: " + num_vertices);
            file_parse_results.WriteLine("Number of triangles: " + num_triangles);
            file_parse_results.WriteLine("Number of normals: " + num_normals);
            file_parse_results.WriteLine("Number of textures: " + num_textures);
            file_parse_results.WriteLine("\n\nThe vertices are (in order): \n");
            foreach (Vertice v in vertices_array)
            {
                file_parse_results.WriteLine("Vertex=> x: " + v.X + "        y: " + v.Y + "       z: " + v.Z);
            }
            file_parse_results.WriteLine("\n\nThe vertex indices for the triangles are (in order): \n");
            foreach (Triangle t in triangles_array)
            {
                file_parse_results.WriteLine("Triangle=> Vertex 1 index: " + t.Vertice1 + "        Vertex 2 index: " + t.Vertice2 + "         Vertex 3 index: " + t.Vertice3);
            }
            file_parse_results.WriteLine("\n\nThe vertex indices for the normals are (in order): \n");
            foreach (Normal n in normals_array)
            {
                file_parse_results.WriteLine("Normal=> x: " + n.X + "        y: " + n.Y + "        z: " + n.Z);
            }
            file_parse_results.WriteLine("\n\nThe model textures are (in order): \n");
            foreach (Texture t in textures_array)
            {
                file_parse_results.WriteLine(t.TextureFile);
            }

            file_parse_results.Close();
            System.Diagnostics.Process.Start("C:\\Users\\Terhands\\Desktop\\Test\\Debug\\Test.exe");


        }
    }
}
