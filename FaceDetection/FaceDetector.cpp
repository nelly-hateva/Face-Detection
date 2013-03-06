#include "stdafx.h"
#include "vector"
#include "queue"
#include "list"
#include "FaceDetector.h"
#include "PercentageWhite.h"
#include "Pixel.h"

using namespace std;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;

Color FaceDetector::FaceColor(Color pixel_color){
	double red =   (double) pixel_color.R;
    double green = (double) pixel_color.G;
    double blue =  (double) pixel_color.B;
    double rgb =   (double) (red + green + blue);

    double normal_red = 0;
    double normal_green = 0;
    double normal_blue = 0;
	if (rgb != 0)
    {
        normal_red =   (double) red / rgb;
        normal_green = (double) green / rgb;
        normal_blue =  (double) blue / rgb;
    }

    if ((red > 95) && (green > 40) && (Math::Max(red, Math::Max(green, blue)) - Math::Min(red, Math::Min(green, blue)) > 15)
                    && (Math::Abs(red - green) > 15) && (red > green) && (red > blue))
        return Color::FromArgb(255, (int)(normal_red * 255), (int)(normal_green * 255), (int)(normal_blue * 255));
    else
        return Color::Black;
}

Bitmap^ FaceDetector::NormalizeImageBrightness(Bitmap^ image){
    for (int x = 0; x < image->Width; x++)
        for (int y = 0; y < image->Height; y++)
        {
            Color pixel_color = image->GetPixel(x, y);
            Color normalized_pixel_color = FaceColor(pixel_color);
            image->SetPixel(x, y, normalized_pixel_color);

        }
    return image;
}

Bitmap^ FaceDetector::GrayScale(Bitmap^ bitmap){
    for(int y = 0; y < bitmap->Height; y++)
        for (int x = 0; x < bitmap->Width; x++)
        {
            Color color = bitmap->GetPixel(x, y);
            int gray_scale = (int)((color.R + color.G + color.B) / 3);
            bitmap->SetPixel(x, y, Color::FromArgb(255, gray_scale, gray_scale, gray_scale));
        }
    return bitmap;
}

Bitmap^ FaceDetector::BlackAndWhite(Bitmap^ image){
    for (int width = 0; width < image->Width; width++)
        for (int heigth = 0; heigth < image->Height; heigth++)
        {
			int red = 0, blue = 0, green = 0;
			Color color = image->GetPixel(width, heigth);
			red =   Convert::ToInt16(color.R);
			green = Convert::ToInt16(color.G);
			blue =  Convert::ToInt16(color.B);
			int rgb = ((red + green + blue)) / 3;
			if (rgb > 56)
			{
			    red = 255;
			    green = 255;
			    blue = 255;
			}
		    else
		    {
			    red = 0;
			    green = 0;
			    blue = 0;
		    }
		    color = Color::FromArgb(255, red, green, blue);
            image->SetPixel(width, heigth, color);
        }
    return image;
}

static int component_number = 1;

vector< vector<int> > FaceDetector::ConectedComponents(vector< vector<int> > matrix){
	queue<Pixel> queue_of_pixels;
	for ( int i = 0; i < matrix.size(); i++)
	{
        for ( int j = 0; j < matrix[0].size(); j++)
        {
	        if (matrix[i][j] == 1)
            {
                component_number++;
		        Pixel pixel = Pixel(i, j);
                queue_of_pixels.push(pixel);
                matrix[i][j] = component_number;
            }
            while (!queue_of_pixels.empty())
            {
                Pixel current_pixel = queue_of_pixels.front();
		        queue_of_pixels.pop();
                if (current_pixel.X - 1 > 0)
                {
                    if (matrix[current_pixel.X - 1][current_pixel.Y] == 1)
                    {
			            Pixel pixel = Pixel(current_pixel.X - 1, current_pixel.Y);
                        queue_of_pixels.push(pixel);
                        matrix[current_pixel.X - 1][current_pixel.Y] = component_number;
                    }
                }

                if (current_pixel.Y - 1 > 0)
                {
                    if (matrix[current_pixel.X][current_pixel.Y - 1] == 1)
                    {
			            Pixel pixel = Pixel(current_pixel.X, current_pixel.Y - 1);
                        queue_of_pixels.push(pixel);
                        matrix[current_pixel.X][current_pixel.Y - 1] = component_number;
                    }
                }

                if (current_pixel.Y < matrix[0].size() - 1)
                {
                    if (matrix[current_pixel.X][current_pixel.Y + 1] == 1)
                    {
			            Pixel pixel = Pixel(current_pixel.X, current_pixel.Y + 1);
                        queue_of_pixels.push(pixel);
                        matrix[current_pixel.X][current_pixel.Y + 1] = component_number;
                    }
                }

                if (current_pixel.X < matrix.size() - 1)
                {
                    if (matrix[current_pixel.X + 1][current_pixel.Y] == 1)
                    {
			            Pixel pixel = Pixel(current_pixel.X + 1, current_pixel.Y);
                        queue_of_pixels.push(pixel);
                        matrix[current_pixel.X + 1][current_pixel.Y] = component_number;
                    }
                }
            }
        }
	}
    return matrix;
}

vector< vector<int> > FaceDetector::Lrtb(vector< vector<int> > matrix){
    while (component_number > 1)
    {
        int right = matrix[0].size(), top = -1, left = -1, bottom = matrix.size();
        double height = 0, width = 0;

        for ( int i = 0; i < matrix[0].size(); i++)
		{
			if (left != -1) break;
            for ( int j = 0; j < matrix.size(); j++)
                if (matrix[j][i] == component_number)
                    if (left == -1)
                        left = i; 
		}

        for ( int i = 0; i < matrix.size(); i++)
	    {
		    if (top != -1) break;
            for ( int j = 0; j < matrix[0].size(); j++)
                if (matrix[i][j] == component_number)
                    if (top == -1)
                        top = i;
	    }

        for ( int i = matrix[0].size() - 1; i > -1; i--)
		{
			if (right != matrix[0].size()) break;
            for ( int j = matrix.size() - 1; j > -1; j--)
                if (matrix[j][i] == component_number)
                    if (right == matrix[0].size())
                        right= i + 1;
		}

        for ( int i = matrix.size() - 1; i > -1; i--)
		{
			if (bottom != matrix.size()) break;
            for ( int j = matrix[0].size() - 1; j > -1; j--)
                if (matrix[i][j] == component_number)
                    if (bottom == matrix.size())
                        bottom = i + 1;
		}

        height = (double) bottom - top;
        width =  (double) right - left;
	    List<PercentageWhite^> ^list_percentage_white = gcnew List<PercentageWhite^>();
	    for ( int i = 0; i < matrix.size(); i++)
            for ( int j = 0; j < matrix[0].size(); j++)
            {
                if (matrix[i][j] == component_number)
                {
		            List<PercentageWhite^> ^filtered_list = gcnew List<PercentageWhite^>();
			        for each(PercentageWhite^ percentage_white in list_percentage_white )
                    {
                        if(percentage_white->getNameComponent() == component_number)
				            filtered_list->Add(percentage_white);
                    }

                    if (filtered_list->Count == 0)
                        list_percentage_white->Add(gcnew PercentageWhite(component_number, 1));
                    else
                    {
                        list_percentage_white->Remove(filtered_list[0]);
                        list_percentage_white->Add(gcnew PercentageWhite(filtered_list[0]->getNameComponent(), filtered_list[0]->getWhiteCounter() + 1));
                    }
                }
            }

	    PercentageWhite^ percentageWhite;
	    for each(PercentageWhite^ percentage_white in list_percentage_white )
            if(percentage_white->getNameComponent() == component_number)
		        percentageWhite = percentage_white;

        if (height / width > 2.5 || height / width < 1 || width * height < 20 || ((double) percentageWhite->getWhiteCounter() / ((height + 5) * (width + 5))) < 0.4)
        {
            for ( int i = 0; i < matrix.size(); i++)
                for ( int j = 0; j < matrix[0].size(); j++)
                    if (matrix[i][j] == component_number)
                        matrix[i][j] = 0;
	    }
        component_number--;
	}
    return matrix;
}

List<RetCoord^> ^FaceDetector::CountComponent(vector< vector<int> > matr){
	List<RetCoord^> ^retCoodr = gcnew List<RetCoord^>();
    vector< vector<int> > matrix = ConectedComponents(matr);
    while (component_number > 1)
    {
        int right = matrix[0].size(), top = -1, left = -1, bottom = matrix.size();

        for (int i = 0; i < matrix[0].size(); i++)
		{
			if (left != -1) break;
            for (int j = 0; j < matrix.size(); j++)
                if (matrix[j][i] == component_number)
                    if (left == -1)
                        left = i;
		}

        for (int i = 0; i < matrix.size(); i++)
		{
			if (top != -1) break;
            for (int j = 0; j < matrix[0].size(); j++)
                if (matrix[i][j] == component_number)
                    if (top == -1)
                        top = i;
		}

	    for (int i = matrix[0].size() - 1; i > -1; i--)
		{
			if (right != matrix[0].size()) break;
            for (int j = matrix.size() - 1; j > -1; j--)
                if (matrix[j][i] == component_number)
                    if (right == matrix[0].size())
                        right = i + 1;
		}

        for (int i = matrix.size() - 1; i > -1; i--)
		{
			if (bottom != matrix.size()) break;
            for (int j = matrix[0].size() - 1; j > -1; j--)
                if (matrix[i][j] == component_number)
                    if (bottom == matrix.size())
                        bottom = i + 1;
		}

        retCoodr->Add(gcnew RetCoord(left, right, top, bottom));
        component_number--;
    }
    return retCoodr;
}