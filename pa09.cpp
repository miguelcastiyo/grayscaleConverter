/// @file <pa09.cpp>
/// @author <Miguel Castillo>
/// @date <12-05-2019>
/// @note I pledge my word of honor that I have abided by the
/// CSN Academic Integrity Policy while completing this assignment.
/// @brief Converts an image from color to gray scale.
/// @note Several hours. Need to remember to code to specifications.
///       Received help from Raul in the software lab, especially when it came 
///       to creating the output file.

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

using namespace std;

//  Struct used to store pixel values of type short. 1 pixel_t = 1 pixel.
struct pixel_t {
    short RED;      //  Number of red.
    short GREEN;    //  Number of green.
    short BLUE;     //  Number of blue
};

const short MAX_WIDTH = 1025;       //  Used as maximum for file size.
const short MAX_HEIGHT = 769;       //  Used as maximum for file size.


//  Function Prototypes
void gray_scale(pixel_t array[MAX_WIDTH][MAX_HEIGHT], short& width,
                short& height);
void create_file(ofstream& outFile, string& output_file, string& magic_number,
                 short& width, short& height, short& max_color,
                 pixel_t array[MAX_WIDTH][MAX_HEIGHT]);
void read_header(ifstream& infile, string& magic_number, short& width,
                 short& height, short& max_color);
void fill_arrays(ifstream& infile, pixel_t array[MAX_WIDTH][MAX_HEIGHT],
                 const short& width, const short& height);

int main(int argc, char *arg[]) {

    //  Declarations
    string magic_number;               //  Stores magic number on file.
    string user_file;                  //  Stores file name of image in color
    string output_file;                //  Stores file name of image in gray
    short width;                       //  Stores image width.
    short height;                      //  Stores image height.
    short max_color;                   //  Stores max color value per pixel.
    size_t location_input;             //  Stores location of ".ppm" if found.
    size_t location_output;            //  Stores location of ".ppm" if found.
    pixel_t array[MAX_WIDTH][MAX_HEIGHT]; //  array used to store RGB pixels.
    bool status  = false;              //  Used as a boolean flag

    ifstream infile;                   //  Opens file stream.
    infile.open(arg[1]);               //  Uses user file name as file to open.


    //  Processing
    if (argc == 1) {
        cout << "Error: Missing input file name" << endl;
    } else if (argc == 2) {
        cout << "Error: Missing output file name" << endl;
    } else if (argc == 3) {
        user_file    = arg[1];
        output_file   = arg[2];
        location_input = user_file.find(".ppm");
        location_output = output_file.find(".ppm");

        if (user_file == output_file || output_file == user_file) {
            cout << "Error: input and output filenames must differ" << endl;
        } else if (location_input == string::npos) {
            cout << "Error: invalid input file format" << endl;
        } else if (location_output == string::npos) {
            cout << "Error: invalid output file format" << endl;
        } else {
            read_header(infile, magic_number, width, height, max_color);

            if (width > MAX_WIDTH || height > MAX_HEIGHT) {
                cout << "Error: input file too large" << endl;
            } else {
                status = true;
                fill_arrays(infile, array, width, height);
                gray_scale(array, width, height);
                infile.close();             //  Closes file after extraction
                ofstream outFile;           //  Stream for new file
                create_file(outFile, output_file, magic_number, width, height, 
                max_color, array);
            }
        }
    }

    return status;
}

/// ---------------------------------------------------------------------------
/// @brief Turns all pixel_t components to the same, computed average, value
///        in order to create a gray scale effect.
///
/// @param [in,out]  array array where original pixel_t
///        values are stored.
/// @param [in] width  value of width read from input file.
/// @param [in] height  value of height read from input file.
///
/// ---------------------------------------------------------------------------
void gray_scale(pixel_t array[MAX_WIDTH][MAX_HEIGHT], short& width,
                short& height) {
    short average;  //  stores value of computed average for gray scaling.

    for (int m = 0; m < width; ++m) {       //  "m" is used as a counter.
        for (int t = 0; t < height; ++t) {  //  "t" is used as a counter.
            average = (array[m][t].RED + array[m][t].GREEN +
                       array[m][t].BLUE) / 3;
            array[m][t].RED = average;
            array[m][t].GREEN = average;
            array[m][t].BLUE = average;
        }

    }

}

/// ---------------------------------------------------------------------------
/// @brief Reads the header of image file in order to store much needed
///        elements for computation and storage.
///
/// @param [in,out]  infile  carries file position.
/// @param [in] magic_number  stores magic number in header for file type.
/// @param [in] width  value of width read from input file.
/// @param [in] height  value of height read from input file.
/// @param [in] max_color  maximum color value each pixel can be.
///
/// ---------------------------------------------------------------------------
void read_header(ifstream& infile, string& magic_number, short& width,
                 short& height, short& max_color) {

    infile >> magic_number;
    infile.ignore(1, '\n');

    if (infile.peek() == '#') {
        infile.ignore(80, '\n');
    }

    infile >> width >> height >> max_color;
}

/// ---------------------------------------------------------------------------
/// @brief Fills the array of type pixel_t with values representing color.
///
/// @param [in,out]  infile  carries file position.
/// @param [in,out]  array array where original pixel_t
///        values are stored.
/// @param [in] width  value of width read from input file.
/// @param [in] height  value of height read from input file.
///
/// ---------------------------------------------------------------------------
void fill_arrays(ifstream& infile, pixel_t array[MAX_WIDTH][MAX_HEIGHT],
                 const short& width, const short& height) {

    for (int m = 0; m < width ; ++m) {       //  "m" is used as a counter.
        for (int t = 0; t < height; ++t) {   //  "t" is used as a counter.
            infile >> array[m][t].RED;
            infile >> array[m][t].GREEN;
            infile >> array[m][t].BLUE;
        }
    }
}

/// ---------------------------------------------------------------------------
/// @brief Creates a new file using the name inputted by the user in the
///        command line. This file will be gray scale.
///
/// @param [in,out] outFile  carries file position.
/// @param [in] output_file  carries name of user-named new file.
/// @param [in] magic_number  stores magic number in header for file type.
/// @param [in] width  value of width read from input file.
/// @param [in] height  value of height read from input file.
/// @param [in] max_color  maximum color value each pixel can be.
/// @param [in] array array where original pixel_t
///        values are stored.
///
/// ---------------------------------------------------------------------------
void create_file(ofstream& outFile, string& output_file, string& magic_number,
                 short& width, short& height, short& max_color,
                 pixel_t array[MAX_WIDTH][MAX_HEIGHT]) {


    outFile.open(output_file);      //  Creates an output file with name.
    outFile << magic_number << endl;
    outFile << "# created by Miguel" << endl;
    outFile << width << " " << height << endl;
    outFile << max_color << endl;

    for (int m = 0; m < width; ++m) {   //  "m" is used as a counter.
        for (int t = 0; t < height; ++t) {  //  "t" is used as a counter.
            outFile << array[m][t].RED <<  " ";
            outFile << array[m][t].GREEN << " ";
            outFile << array[m][t].BLUE << " ";
        }

        outFile << endl;
    }
    
    outFile.close();
}

