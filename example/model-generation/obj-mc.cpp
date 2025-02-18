#include <fstream>
#include <iostream>
#include <limits>
#include <mcpp/mcpp.h>
#include <string>
#include <vector>

struct Vec3 {
  float x, y, z;
};

struct Face {
  int first, second, third;
};

class Model {
public:
  Model(const std::string& filename);
  ~Model() {
    if (_file.is_open()) {
      _file.close();
    }
  }

  void BuildModel(mcpp::MinecraftConnection& mc);
  void BuildPointCloud(mcpp::MinecraftConnection& mc);

  void Scale(int scale);
  void SetPosition(mcpp::Coordinate position);

private:
  bool IsWithin(Vec3& position);

  Vec3 sub(Vec3 a, Vec3 b);
  float dot(Vec3 a, Vec3 b);
  Vec3 cross(Vec3 a, Vec3 b);

  std::ifstream _file;

  std::vector<Vec3> _vertices;
  std::vector<Face> _faces;

  mcpp::Coordinate _position;
};

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " filename.obj scale" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int scale = std::stoi(argv[2]);

  mcpp::MinecraftConnection mc;

  Model* model = new Model(filename);
  model->SetPosition(mc.getPlayerPosition());
  model->Scale(scale);
  model->BuildModel(mc);      // Fills the model
  model->BuildPointCloud(mc); // Places blocks at vertices

  return 0;
}

Model::Model(const std::string& filename) {
  _file.open(filename, std::ios::binary);
  if (!_file.is_open()) {
    throw std::runtime_error("Error opening file");
  }

  std::string line;
  while (std::getline(_file, line)) {
    if (line.substr(0, 2) == "v ") {
      Vec3 vertex;
      std::sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      _vertices.push_back(vertex);
    } else if (line.substr(0, 2) == "f ") {
      Face face;
      if (line.find(std::string("//")) != std::string::npos) {
        std::sscanf(line.c_str(), "f %d//%*d %d//%*d %d//%*d", &face.first, &face.second,
                    &face.third);
      } else if (line.find(std::string("/")) != std::string::npos) {
        std::sscanf(line.c_str(), "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &face.first, &face.second,
                    &face.third);
      } else {
        std::sscanf(line.c_str(), "f %d %d %d", &face.first, &face.second, &face.third);
      }

      _faces.push_back(face);
    }
  }
}

void Model::Scale(int scale) {
  Vec3 min;
  Vec3 max;
  min.x = min.y = min.z = std::numeric_limits<float>::max();
  max.x = max.y = max.z = std::numeric_limits<float>::lowest();

  for (const Vec3& vertex : _vertices) {
    min.x = std::min(min.x, vertex.x);
    min.y = std::min(min.y, vertex.y);
    min.z = std::min(min.z, vertex.z);

    max.x = std::max(max.x, vertex.x);
    max.y = std::max(max.y, vertex.y);
    max.z = std::max(max.z, vertex.z);
  }

  float size = std::max(max.x - min.x, std::max(max.y - min.y, max.z - min.z));
  for (Vec3& vertex : _vertices) {
    vertex.x = (vertex.x - min.x) / size * scale;
    vertex.y = (vertex.y - min.y) / size * scale;
    vertex.z = (vertex.z - min.z) / size * scale;
  }
}

void Model::BuildPointCloud(mcpp::MinecraftConnection& mc) {
  for (const Vec3& vertex : _vertices) {
    mcpp::Coordinate blockPosition =
        mcpp::Coordinate(_position.x + vertex.x, _position.y + vertex.y, _position.z + vertex.z);
    mc.setBlock(blockPosition, mcpp::Blocks::GRAY_WOOL);
  }
}

void Model::BuildModel(mcpp::MinecraftConnection& mc) {
  Vec3 min;
  Vec3 max;
  min.x = min.y = min.z = std::numeric_limits<float>::max();
  max.x = max.y = max.z = std::numeric_limits<float>::lowest();

  for (const Vec3& vertex : _vertices) {
    min.x = std::min(min.x, vertex.x);
    min.y = std::min(min.y, vertex.y);
    min.z = std::min(min.z, vertex.z);

    max.x = std::max(max.x, vertex.x);
    max.y = std::max(max.y, vertex.y);
    max.z = std::max(max.z, vertex.z);
  }

  for (int x = min.x; x < max.x; x++) {
    for (int y = min.y; y < max.y; y++) {
      for (int z = min.z; z < max.z; z++) {
        Vec3 position = {(float)x, (float)y, (float)z};
        if (IsWithin(position)) {
          mcpp::Coordinate blockPosition =
              mcpp::Coordinate(_position.x + x, _position.y + y, _position.z + z);
          mc.setBlock(blockPosition, mcpp::Blocks::GRAY_CONCRETE);
        }
      }
    }
  }
}

bool Model::IsWithin(Vec3& position) {
  const float EpsilonFloat = 0.0000001;
  const Vec3 directions[4] = {{0.0, 1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}};

  int crosses = 0;

  for (Vec3 direction : directions) {
    int intersects = 0;

    for (const Face& face : _faces) {
      // 1 based indexing -.-
      Vec3 v1 = _vertices[face.first - 1];
      Vec3 v2 = _vertices[face.second - 1];
      Vec3 v3 = _vertices[face.third - 1];

      Vec3 edge1 = sub(v2, v1);
      Vec3 edge2 = sub(v3, v1);

      Vec3 h = cross(direction, edge2);
      float det = dot(edge1, h);

      if (std::abs(det) < EpsilonFloat)
        continue; // parallel

      float f = 1.0 / det;
      Vec3 s = sub(position, v1);
      float u = f * dot(s, h);

      if (u < 0.0 || u > 1.0)
        continue; // outside face

      Vec3 q = cross(s, edge1);
      float v = f * dot(direction, q);

      if (v < 0.0 || u + v > 1.0)
        continue; // outside face

      float t = f * dot(edge2, q);

      if (t > EpsilonFloat)
        intersects++;
    }

    if (intersects % 2 == 1)
      crosses++;
  }

  return (crosses >= 2);
}

Vec3 Model::sub(Vec3 a, Vec3 b) { return Vec3{a.x - b.x, a.y - b.y, a.z - b.z}; }
float Model::dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec3 Model::cross(Vec3 a, Vec3 b) {
  return Vec3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

void Model::SetPosition(mcpp::Coordinate position) {
  _position = position;
  _position.x += 16;
  _position.z += 16;
};
