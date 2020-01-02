#include "SettingLoader.h"


SettingLoader::SettingLoader()
{
}


SettingLoader::~SettingLoader()
= default;

HRESULT SettingLoader::CreateVertexShaders(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::string fileName ) {

	static bool createLayout = true;
	ID3DBlob* pVSBlob = nullptr;



	hr = CompileShaderFromFile(std::wstring(fileName.begin(), fileName.end()).c_str(), "VS", "vs_4_0", &pVSBlob);
	
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}
	
	ID3D11VertexShader* v = nullptr;
	
	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &v);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}
	
	_vertexShaderList.push_back(v);

	if(createLayout) {

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &_pVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr))
			return hr;

		// Set the input layout
		pImmediateContext->IASetInputLayout(_pVertexLayout);
		
	}

	return S_OK;

}

HRESULT SettingLoader::CreatePixelShaders(HRESULT hr, ID3D11Device * pd3dDevice, std::string fileName) {

	ID3DBlob* pPSBlob = nullptr;
	
	hr = CompileShaderFromFile(std::wstring(fileName.begin(), fileName.end()).c_str(), "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	ID3D11PixelShader* p = nullptr;

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &p);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	_pixelShaderList.push_back(p);

	return S_OK;

}

void SettingLoader::FileLoader(HRESULT hr, ID3D11Device* pd3Device, ID3D11DeviceContext* pImmediateContext) {

	std::ifstream fin("settings.ini");
	
	std::string str;

	

	std::string field = "";
	
	while (getline(fin, str)) {

		
		if (str.empty()) {
			
			continue;
		}

		if (str[0] == '#') {

			continue;
		}

		if (str[0] == '[') {

			field = str.substr(1, str.find(']') - 1);
			continue;
		}

		if (field == "Geometries") {

			const auto i = str.find('=');
			ObjLoader(str.substr(i + 2, str.size() - i));
			continue;

		}

		if (field == "Objects") {

			ShapeCoordinates object;

			auto i = str.find('=');

			object.Pos.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			auto x = str.substr(i + 2, str.find(',', i + 1) - i + 1);
			object.Pos.y = stof( x );
			i = str.find(',', i + 1);
			object.Pos.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);

			
			i = str.find('=');

			object.Rot.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			object.Rot.y = stof(str.substr(i + 2, str.find(',', i + 1) - i + 1));
			i = str.find(',', i + 1);
			object.Rot.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);
			
			i = str.find('=');

			object.Scal.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			object.Scal.y = stof(str.substr(i + 2, str.find(',', i + 1) - i + 1));
			i = str.find(',', i + 1);
			object.Scal.z = stof(str.substr(i + 2, str.size() - i + 1));

			_objectCoordinates.push_back(object);

			continue;

		}

		//if (field == "Lights") {

		//	auto i = str.find('=');

		//	cout << str.substr(0, i - 1) << " = ";
		//	cout << str.substr(i + 2, str.size() - i + 1) << endl;
		//	getline(fin, str);

		//	i = str.find('=');

		//	cout << stof(str.substr(i + 2, str.find(',') - i)) << " ";
		//	i = str.find(',');
		//	cout << stof(str.substr(i + 2, str.find(',') - i + 1)) << " ";
		//	i = str.find(',', i);
		//	cout << stof(str.substr(i + 2, str.size() - i + 1)) << endl;
		//	getline(fin, str);

		//	i = str.find('=');

		//	cout << stof(str.substr(i + 2, str.find(',') - i)) << " ";
		//	i = str.find(',');
		//	cout << stof(str.substr(i + 2, str.find(',') - i + 1)) << " ";
		//	i = str.find(',', i);
		//	cout << stof(str.substr(i + 2, str.size() - i + 1)) << endl;
		//	getline(fin, str);


		//	continue;

		//}

		if (field == "Cameras") {

			CameraCoordinates camera;

			auto i = str.find('=');

			auto x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			auto y = stof(str.substr(i + 2, str.find(',', i + 1) - i + 1));
			i = str.find(',', i+1);
			auto z = stof(str.substr(i + 2, str.size() - i + 1));

			camera.Eye = DirectX::XMVectorSet(x, y, z, 0.0f);
			
			getline(fin, str);
			i = str.find('=');

			x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			y = stof(str.substr(i + 2, str.find(',', i + 1) - i + 1));
			i = str.find(',', i + 1);
			z = stof(str.substr(i + 2, str.size() - i + 1));

			camera.At = DirectX::XMVectorSet(x, y, z, 0.0f);
			
			getline(fin, str);			
			i = str.find('=');

			x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			y = stof(str.substr(i + 2, str.find(',', i + 1) - i + 1));
			i = str.find(',', i + 1);
			z = stof(str.substr(i + 2, str.size() - i + 1));

			camera.Up = DirectX::XMVectorSet(x, y, z, 0.0f);
			

			_cameraCoordinates.push_back(camera);

			continue;

		}

		if (field == "VertexShaders") {
		
			CreateVertexShaders(hr, pd3Device, pImmediateContext, str);
		
			continue;
		
		}
		
		if (field == "PixelShaders") {
		
			CreatePixelShaders(hr, pd3Device, str);
		
			continue;
		
		}


	}

	

	
}


HRESULT SettingLoader::CompileShaderFromFile(const WCHAR* szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}



void SettingLoader::ObjLoader(std::string filename) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);
	const aiMesh* mesh = scene->mMeshes[0];

	ShapeGeometry shape;
	

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

		SimpleVertex vertex;

		vertex.Pos.x = mesh->mVertices[i].x;
		vertex.Pos.y = mesh->mVertices[i].y;
		vertex.Pos.z = mesh->mVertices[i].z;
		vertex.Color.x = 1.0f;
		vertex.Color.y = 1.0f;
		vertex.Color.z = 1.0f;
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;
		//vertex.TexCoord.x = teapotMesh->mTextureCoords[i]->x;
		//vertex.TexCoord.y = teapotMesh->mTextureCoords[i]->y;
		vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
		vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
		
		shape.vertices.push_back(vertex);
		
		
	}
	

	std::vector<UINT16> indices;

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		
		const aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) shape.indices.push_back(face.mIndices[j]);


	}

	_shapeGeometries.push_back(shape);
	
}

void SettingLoader::CreateParticleGeometry() {


	ShapeGeometry particle;

	particle.vertices.push_back({ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	particle.vertices.push_back({ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	particle.vertices.push_back({ DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	particle.vertices.push_back({ DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });

	particle.indices.push_back(3);
	particle.indices.push_back(1);
	particle.indices.push_back(0);
	particle.indices.push_back(2);
	particle.indices.push_back(1);
	particle.indices.push_back(3);

	_shapeGeometries.push_back(particle);
}

