/* https://www.a1k0n.net/2006/09/15/obfuscated-c-donut.html */
/* https://www.a1k0n.net/2006/09/15/obfuscated-c-donut.html */
/* https://www.a1k0n.net/2011/07/20/donut-math.html */

#include <stdio.h>
#include <string.h>
#include <math.h>


const float phiStep = 0.05;
const float thetaStep = 0.01;
const float TWO_PI = 6.28;
const char *shade_chars = ".,-~:;=!*#$@";
const int width = 80;
const int height = 22;
const size_t buffer_size = height * width;

void display_buffer(size_t height,size_t width,char *buffer){
    printf("\x1b[H");
    for (int k = 0; height*width >= k; k++)
        putchar(k % width ? buffer[k] : 10); // 10 => ASCII(\n)
}

void iterate_buffer(int height,int width,float A,float B,char *buffer,size_t buffer_size){
    float z[buffer_size];

    memset(buffer, ' ', buffer_size);
    memset(z, 0, buffer_size * sizeof(float)); // buffer_size * 4 = 7040
    for (float phi = 0; phi < TWO_PI; phi += phiStep){
        for (float theta = 0; theta < TWO_PI; theta += thetaStep) {
            float sin_theta = sin(theta);
            float cos_phi = cos(phi);
            float sin_A = sin(A);
            float sin_phi = sin(phi);
            float cos_A = cos(A);
            float h = cos_phi + 2;
            float D = 1 / (sin_theta * h * sin_A + sin_phi * cos_A + 5);
            float cos_theta = cos(theta);
            float cos_B = cos(B);
            float n = sin(B);
            float t = sin_theta * h * cos_A - sin_phi * sin_A;
            int x = (width / 2) + 30 * D * (cos_theta * h * cos_B - t * n);
            int y = (height / 2 - 1) + 15 * D * (cos_theta * h * n + t * cos_B);
            int o = x + width * y;
            int N = 8 * ((sin_phi * sin_A - sin_theta * cos_phi * cos_A) * cos_B - sin_theta * cos_phi * sin_A - sin_phi * cos_A - cos_theta * cos_phi * n);
            if (height > y && y > 0 && x > 0 && width > x && D > z[o]) {
                z[o] = D;
                buffer[o] = shade_chars[N > 0 ? N : 0];
            }
        }
    }
}

int main() {
    float A = 0, B = 0;
    char buffer[buffer_size];

    printf("\x1b[2J");
    /* printf("\x1b[2K"); */

    while (1){
        iterate_buffer(height,width,A,B,buffer,buffer_size);
        display_buffer(height,width,buffer);
        A += 0.04;
        B += 0.02;
    }
}

