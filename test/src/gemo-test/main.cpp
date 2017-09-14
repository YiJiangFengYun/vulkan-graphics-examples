
#include <framework/global.hpp>
#include <foundation/gemo.hpp>

int main()
{
	gfw::init(); //init framework module, and it also init foundation and graphics modules at the same time.

	fd::Bounds<glm::vec3> bounds(glm::vec3(0, 0, 0), glm::vec3(10, 10, 10));

	return 0;
}