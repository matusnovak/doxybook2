#include <iostream>
#include <memory>
//! [TestTest]
#include "Engine.hpp"

// A dummy example 
int main() {
	// Create pixels buffer
	const auto pixels = std::make_unique<uint8_t[]>(new uint8_t[1024*1024*3]);
	fillData(*pixels, "path/to/texture.png");

	// Create a texture out of the pixels
	Engine::Graphics::Texture2D texture(1024, 1024, *data);

	// Done
	return 0;
}
//! [TestTest]
