#include "lib/sandpile/sandpile.h"

int main(int argc, char** argv) {
    Sandpile sandpile;
    sandpile.Parse(argc, argv);
    sandpile.Create();
    sandpile.AddEllipse(std::make_pair(50, 100), std::make_pair(100, 100), 60);
    sandpile.Disperse();

    return 0;
}
