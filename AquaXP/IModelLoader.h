//#pragma once
//
//#include "Texture.h"
//
//namespace AquaXP
//{
//
//    struct ModelLoaderVertex
//    {
//        DirectX::XMFLOAT3 pos;
//        DirectX::XMFLOAT3 normal;
//        DirectX::XMFLOAT4 tangent;
//        std::vector<DirectX::XMFLOAT2> texcoord;
//        std::vector<DirectX::XMFLOAT4> color;
//        std::vector<DirectX::XMUINT4> joints;
//        std::vector< DirectX::XMFLOAT4> weights;
//    };
//
//    template<typename T>
//    struct ModelLoaderModel
//    {
//        std::vector<T> vertices;
//        std::vector<u32> indices;
//        std::vector<Texture const*> textures;
//
//    };
//
//    template<typename T>
//    using VertexTransformer = std::function<T(ModelLoaderVertex)>;
//
//    template<typename T>
//    class IModelLoader
//    {
//    public:
//        IModelLoader();
//
//        IModelLoader& setDefaultPos(DirectX::XMFLOAT3 position);
//        IModelLoader& setDefaultNormal(DirectX::XMFLOAT3 normal);
//        IModelLoader& setDefaultTangent(DirectX::XMFLOAT4 tangent);
//        /* No default texcoords, colors, or joints since each one will just be an empty list*/
//
//        IModelLoader& setVertexHandler(VertexTransformer<T> transformer);
//
//        std::vector<T> loadMeshData(std::string const& path);
//    };
//}