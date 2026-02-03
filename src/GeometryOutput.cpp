#include "GeometryOutput.h"
#include <fstream>

void write_box_vtp(double xmin, double xmax,
                   double ymin, double ymax,
                   double zmin, double zmax,
                   const std::string& filename) {

    std::ofstream file(filename);

    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    file << "<PolyData>\n";
    file << "<Piece NumberOfPoints=\"8\" NumberOfPolys=\"6\">\n";

    // Points
    file << "<Points>\n";
    file << "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    file << xmin << " " << ymin << " " << zmin << " ";
    file << xmax << " " << ymin << " " << zmin << " ";
    file << xmax << " " << ymax << " " << zmin << " ";
    file << xmin << " " << ymax << " " << zmin << " ";
    file << xmin << " " << ymin << " " << zmax << " ";
    file << xmax << " " << ymin << " " << zmax << " ";
    file << xmax << " " << ymax << " " << zmax << " ";
    file << xmin << " " << ymax << " " << zmax << "\n";
    file << "</DataArray>\n";
    file << "</Points>\n";

    // Faces (6 quads)
    file << "<Polys>\n";
    file << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
    file << "0 1 2 3  4 5 6 7  "
         << "0 1 5 4  2 3 7 6  "
         << "0 3 7 4  1 2 6 5\n";
    file << "</DataArray>\n";

    file << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n";
    file << "4 8 12 16 20 24\n";
    file << "</DataArray>\n";
    file << "</Polys>\n";

    file << "</Piece>\n";
    file << "</PolyData>\n";
    file << "</VTKFile>\n";

    file.close();
}
