namespace ice_engine
{
namespace detail
{

inline std::tuple<std::vector<glm::vec3>, std::vector<uint32>> generateCube(const float32 size = 1.0f)
{
	// original code here: https://learnopengl.com/code_viewer.php?code=advanced/cubemaps_skybox_data
	std::vector<glm::vec3> vertices = {
		{-1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};

	std::vector<uint32> indices = {
		0, 1, 2,
		2, 3, 0,

		4, 1, 0,
		0, 5, 4,

		2, 6, 7,
		7, 3, 2,

		4, 5, 7,
		7, 6, 4,

		0, 3, 7,
		7, 5, 0,

		1, 4, 2,
		2, 4, 6
	};

	return std::make_tuple(std::move(vertices), std::move(indices));
}

}
}
