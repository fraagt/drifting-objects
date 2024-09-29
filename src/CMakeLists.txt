# Define the list of source files
set(SOURCES
        main.cpp
        Application.cpp
        Application.hpp
        utils/Common.hpp
        Mesh.cpp
        Mesh.hpp
        Transform.hpp
        utils/Utils.hpp
        utils/Utils.cpp
        Camera.hpp
        Vertex.hpp
        Shader.hpp
        Shader.cpp
        utils/not_defined.hpp
        utils/files.hpp
        glfwxx/KeyCode.hpp
        glfwxx/BufferUsage.hpp
)

# Prepend "src/" to every source file
list(TRANSFORM SOURCES PREPEND "src/")

# Propagate the SOURCES variable to the parent scope
set(SOURCES ${SOURCES} PARENT_SCOPE)