#include "FileSystem.h"
#include <cstdlib>

const char* logl_root = "D:/gitDeom/JGL";

//D:\gitDeom\JGL\JGL_MeshLoader
std::string FileSystem::getPath(const std::string& path)
{
	static Builder pathBuilder = getPathBuilder();

	return (*pathBuilder)(path);

}


std::string const& FileSystem::getRoot()
{
	static char const* envRoot = getenv("LOGL_ROOT_PATH");
	static char const* givenRoot = (envRoot != nullptr ? envRoot : logl_root);
	static std::string root = (givenRoot != nullptr ? givenRoot : "");
	return root;
}


FileSystem::Builder FileSystem::getPathBuilder()
{
	if (getRoot() != "")
		return &FileSystem::getPathRelativeRoot;
	else
		return &FileSystem::getPathRelativeBinary;
}


std::string FileSystem::getPathRelativeRoot(const std::string& path)
{
	return getRoot() + std::string("/") + path;
}


std::string FileSystem::getPathRelativeBinary(const std::string& path)
{
	return "../../../" + path;
}