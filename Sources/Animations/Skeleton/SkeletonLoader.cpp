#include "SkeletonLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace fl
{
	SkeletonLoader::SkeletonLoader(LoadedValue *libraryControllers, const std::vector<std::string> &boneOrder) :
		m_armatureData(nullptr),
		m_boneOrder(boneOrder),
		m_jointCount(0),
		m_headJoint(nullptr)
	{
		m_armatureData = libraryControllers->GetChild("visual_scene")->GetChildWithAttribute("node", "-id", "Armature");
		auto headNode = m_armatureData->GetChild("node");
		m_headJoint = LoadJointData(headNode, true);
	}

	SkeletonLoader::~SkeletonLoader()
	{
		delete m_headJoint;
	}

	JointData *SkeletonLoader::LoadJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		JointData *joint = ExtractMainJointData(jointNode, isRoot);

		if (jointNode->GetChild("node"))
		{
			if (!jointNode->GetChild("node")->GetChild("-id"))
			{
				for (auto &childNode : jointNode->GetChild("node")->GetChildren())
				{
					joint->AddChild(LoadJointData(childNode, false));
				}
			}
			else if (!jointNode->GetChild("node")->GetChildren().empty())
			{
				joint->AddChild(LoadJointData(jointNode->GetChild("node"), false));
			}
		}

		return joint;
	}

	JointData *SkeletonLoader::ExtractMainJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		std::string nameId = jointNode->GetChild("-id")->GetString();
		auto index = GetBoneIndex(nameId);
		auto matrixData = FormatString::Split(jointNode->GetChild("matrix")->GetChild("#text")->GetString(), " ");

		Matrix4 transform = Matrix4();

		for (unsigned int i = 0; i < matrixData.size(); i++)
		{
			transform.m_linear[i] = std::stof(matrixData[i]);
		}

		transform = transform.Transpose();

		if (isRoot)
		{
			// Because in Blender z is up, but the engine is y up.
			transform *= MeshAnimated::CORRECTION;
		}

		m_jointCount++;
		return new JointData(index, nameId, transform);
	}

	int SkeletonLoader::GetBoneIndex(const std::string &name)
	{
		for (unsigned int i = 0; i < m_boneOrder.size(); i++)
		{
			if (m_boneOrder[i] == name)
			{
				return i;
			}
		}

		return -1;
	}
}
