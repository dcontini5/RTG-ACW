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

	std::string variable;

	std::ifstream fin("settings.ini");

	std::string str;

	getline(fin, str);

	std::string field;

	while (fin) {

		if (str.empty()) {
			getline(fin, str);
			continue;
		}

		if (str[0] == '[') {

			field = str.substr(1, str.find(']') - 1);
			getline(fin, str);
		}

		if (field == "Geometries") {

			const auto i = str.find('=');
			ObjLoader(str.substr(i + 2, str.size() - i));
			getline(fin, str);
			continue;

		}

		if (field == "Objects") {

			ShapeCoordinates object;

			auto i = str.find('=');

			object.Pos.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			object.Pos.y = stof(str.substr(i + 2, str.find(',') - i + 1));
			i = str.find(',', i);
			object.Pos.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);

			i = str.find('=');

			object.Rot.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			object.Rot.y = stof(str.substr(i + 2, str.find(',') - i + 1));
			i = str.find(',', i);
			object.Rot.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);
			
			i = str.find('=');

			object.Scal.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			object.Scal.y = stof(str.substr(i + 2, str.find(',') - i + 1));
			i = str.find(',', i);
			object.Scal.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);

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

			camera.Eye.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			camera.Eye.y = stof(str.substr(i + 2, str.find(',') - i + 1));
			i = str.find(',', i);
			camera.Eye.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);

			i = str.find('=');

			camera.At.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			camera.At.y = stof(str.substr(i + 2, str.find(',') - i + 1));
			i = str.find(',', i);
			camera.At.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);

			i = str.find('=');

			camera.Up.x = stof(str.substr(i + 2, str.find(',') - i));
			i = str.find(',');
			camera.Up.y = stof(str.substr(i + 2, str.find(',') - i + 1));
			i = str.find(',', i);
			camera.Up.z = stof(str.substr(i + 2, str.size() - i + 1));
			getline(fin, str);

			_cameraCoordinates.push_back(camera);

			continue;

		}

		if (field == "VertexShaders") {

			CreateVertexShaders(hr, pd3Device, pImmediateContext, str);
			getline(fin, str);

			continue;

		}

		if (field == "PixelShaders") {

			CreatePixelShaders(hr, pd3Device, str);
			getline(fin, str);

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
	const aiMesh* teapotMesh = scene->mMeshes[0];

	std::vector<SimpleVertex> mesh_vertices;

	for (auto i = 0; i<teapotMesh->mNumVertices; i++) {

		SimpleVertex vertex;

		vertex.Pos.x = teapotMesh->mVertices[i].x;
		vertex.Pos.y = teapotMesh->mVertices[i].y;
		vertex.Pos.z = teapotMesh->mVertices[i].z;

		vertex.TexCoord.x = teapotMesh->mNormals[i].x;
		vertex.TexCoord.x = teapotMesh->mTextureCoords[0]->x;

		
	}
	

	
}