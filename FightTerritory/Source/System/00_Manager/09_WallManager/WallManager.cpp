#include "WallManager.h"

#include "GameObject//02_StaticMeshObject//05_Wall//Wall.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"

using json = nlohmann::json;

WallManager::WallManager()
	: ManagerBase	()
{
}

WallManager::~WallManager()
{
	Release();
}

void WallManager::Update()
{
	//全ての壁.
	for (auto& wall : m_Wall)
	{
		wall->Update();
	}
}

void WallManager::Draw()
{
	ImGui::Begin("Wall Editor");

	if (ImGui::Button(JAPANESE("新しい壁を生成")))
	{
		m_Wall.push_back(std::make_unique<Wall>());
	}
	//枠を区切る,
	ImGui::SameLine();
	if (ImGui::Button("Save Data"))
	{
		//読み込んだjsonへ保存する.
		SaveData("Data\\json\\WallPosition\\WallPosition.json");
	}

	ImGui::Separator();

	for (int i = 0; i < (int)m_Wall.size(); i++)
	{
		std::string Label = "Wall" + std::to_string(i);
		if (ImGui::TreeNode(Label.c_str()))
		{
			//削除.
			if (ImGui::Button(JAPANESE("削除")))
			{
				m_Wall.erase(m_Wall.begin() + i);
				ImGui::TreePop();
				ImGui::End();
				return;
			}
			//GameObjectクラスの座標・サイズ・向きを取得して編集する.
			//位置の変更をする.
			D3DXVECTOR3 Pos = m_Wall[i]->GetPosition();
			if (ImGui::SliderFloat3("Positon", (float*)&Pos, -100.0f, 100.0f))
			{
				m_Wall[i]->SetPosition(Pos);
			}
			D3DXVECTOR3 Scale = m_Wall[i]->GetScale();
			if (ImGui::SliderFloat3("Scale", (float*)&Scale, -100.0f, 100.0f))
			{
				m_Wall[i]->SetScale(Scale);
			}
			D3DXVECTOR3 Rot = m_Wall[i]->GetRotation();
			if (ImGui::SliderFloat3("Rotation", (float*)&Rot, -360.0f, 360.0f))
			{
				m_Wall[i]->SetRotation(Rot);
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

	for (auto& wall : m_Wall) 
	{ 
		wall->Draw();
	}
}

void WallManager::Create()
{
	//jsonファイルの読み込み.
	LoadData("Data\\json\\WallPosition\\WallPosition.json");
}

void WallManager::Release()
{
	m_Wall.clear();
}

void WallManager::SaveData(const std::string& Path)
{
	json Root = json::array();
	for (const auto& wall : m_Wall)
	{
		D3DXVECTOR3 p = wall->GetPosition();
		D3DXVECTOR3 s = wall->GetScale();
		D3DXVECTOR3 r = wall->GetRotation();

		Root.push_back(
			{
				{"pos", {p.x, p.y, p.z}},
				{"scl", {s.x, s.y, s.z}},
				{"rot", {r.x, r.y, r.z}}
			});
	}
	std::ofstream o(Path);
	if (o.is_open())
	{
		o << std::setw(4) << Root << std::endl;
	}
}

void WallManager::LoadData(const std::string& Path)
{
	std::ifstream i(Path);
	if (!i.is_open())
	{
		return;
	}

	json Root;
	i >> Root;
	m_Wall.clear();
	for (auto& item : Root)
	{
		auto wall = std::make_unique<Wall>();
		wall->SetPosition({ item["pos"][0], item["pos"][1], item["pos"][2] });
		wall->SetScale({ item["scl"][0], item["scl"][1], item["scl"][2] });
		wall->SetRotation({ item["rot"][0], item["rot"][1], item["rot"][2] });
		m_Wall.push_back(std::move(wall));
	}
}
