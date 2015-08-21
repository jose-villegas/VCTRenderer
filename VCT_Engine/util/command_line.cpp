#include "stdafx.h"

void ConsoleProgressBar(const std::string &title, int barWidth, int index,
                        int last)
{
    std::cout << title + " [";
    float progress = (1.0f / last) * (index + 1);
    int pos = barWidth * progress;

    for(int i = 0; i < barWidth; ++i)
    {
        if(i < pos) std::cout << "=";
        else if(i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

