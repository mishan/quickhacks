/*
 * Copyright (C) 2012-2013 Misha Nasledov <misha@nasledov.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double get_distance (int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

void render_circle (int radius, double tolerance)
{
    int diameter = (radius*2)+1;
    char matrix[diameter][diameter*2];
    int x, y;

    /* clear circle with whitespace */
    for (y = 0; y < diameter; y++) {
        memset(matrix[y], ' ', diameter*2);
    }

    /* a circle is the set of points from the center that are #radius points away from
       the center */
    for (y = 0; y < diameter; y++) {
        for (x = 0; x < diameter*2; x++) {
            double distance = get_distance(x/2, y, radius, radius);
            double radialDistance = abs(distance-radius);
            if (radialDistance <= tolerance) {
                matrix[y][x] = '*';
            }
        }

        for (x = 0; x < diameter*2; x++) {
            printf("%c", matrix[y][x]);
        }
        printf("\n");
    }

    
}

/* print out an ASCII circle, input is a radius */
int main (int argc, char *argv[])
{
    int radius;
    double tolerance;

    if (argc < 3) {
        printf("Usage: %s [radius] [tolerance]\n", argv[0]);
        return 1;
    }

    radius = atoi(argv[1]);
    tolerance = atof(argv[2]);

    render_circle(radius, tolerance);

    return 0;
}
