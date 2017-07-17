#include"ctogui.h"

char Map[40][60];

void Update()
{
    CtoGui::DrawScreen((char*)Map);
}

int main(int argc, char *argv[])
{
    CtoGui::Init(&argc, argv, 600, 400, "Test");
    CtoGui::SetImgSize(10, 10);
    CtoGui::SetImg('*', "img\\star.bmp");
    CtoGui::SetScreenSize(60,40);
    CtoGui::SetUpdateScreenFunc(&Update);

    CtoGui::BeginPlay();
    return 0;
}
