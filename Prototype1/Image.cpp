#include "Image.h"
Image::Image(unsigned int _texture,float _x,float _y) : path(_x,_y),texture(_texture){
    Pos = glm::vec4(_x,_y,0, 0);
    Scale = glm::vec4(0,0,0,0);
    Color = glm::vec4(0, 0, 0, 0);
    angle = 0;
}
/// <summary>
/// Funkcja  (0;255)->(0;1)
/// </summary>
/// <param name="_rgba">Wartosc od 0 do 255</param>
/// <returns></returns>
inline glm::vec4 rgba_to_4f(glm::vec4 &vec) { 
    vec.x /= 255;
    vec.y /= 255;
    vec.z /= 255;
    vec.w /= 255;
    return vec;
}
void Image::rand_color() {
    int r = rand() % 6;
    glm::vec4 rcolor;
    switch (r) {
    case 0://tangerine
        rcolor = glm::vec4(250, 129, 40, 255);
        break;
    case 1://pink
        rcolor = glm::vec4(222, 124, 235, 255);
        break;
    case 2://cyan
        rcolor = glm::vec4(113, 235, 229, 255);
        break;
    case 3://violet
        rcolor = glm::vec4(143, 121, 252, 255);
        break;
    case 4://lime
        rcolor = glm::vec4(174, 243, 89, 255);
        break;
    case 5://candy
        rcolor = glm::vec4(210, 20, 4, 255);
        break;
    }   
    Color = rgba_to_4f(rcolor);
}
Image::ImagePath::ImagePath(float sx, float sy):a(0), b(0), s(1),kierunek(1),speed(PATH_SPEED),x(0) {  
    //Ustal pozycje w ktorej kliknieto mysz
    originx = sx; originy = sy;
    //losuj kierunek w lewo czy w prawo
    kierunek = rand() % 2;
    if (kierunek == 0) {kierunek = -1; }
    //wylosuj punkt najwyzszy czyli b
    b = (PATH_RANGE_B * (rand() / (double)(RAND_MAX)+1)) + 0.05;
    //losuj mnoznik // s = [10;50)
    s = (PATH_RANGE_S*rand() / (double)(RAND_MAX)+1)+PATH_RANGE_T;
    //policz odleglosc przeciecia z osia X przez delte
    double delta = sqrt(4 * s * b);
    a = (-1 * b - (kierunek *delta)) / (2 * s);
}
int Image::updatePath() {
    if ( Pos.y < -2.0f|| Pos.x > 2.0f || Pos.x < -2.0f || Pos.y > 2.0f ) { return true; }  //sprawdz czy wylecial poza okno - jesli tak usun
    if (path.kierunek == 1) {//w prawo      
        path.x += path.speed;
        Pos.x = path.originx + path.x;
        Pos.y = -1 * path.s * (path.a + path.x) * (path.a + path.x) + path.b + path.originy;
        angle += PI / PATH_ROTATE;
    }
    else {//w lewo
        path.x -= path.speed;
        Pos.x = path.originx + path.x;
        Pos.y = -1 * path.s * (path.a + path.x) * (path.a + path.x) + path.b + path.originy;
        angle -= PI / PATH_ROTATE;
    }
    return false;
}