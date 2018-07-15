#pragma once

#include <string>
#include <utility>
#include <optional>
#include <vector>
#include <map>
#include "Helpers/FormatString.hpp"

namespace fl
{
	class FL_EXPORT LoadedValue
	{
	private:
		LoadedValue *m_parent;
		std::vector<LoadedValue *> m_children;

		std::string m_name;
		std::string m_value;
		std::map<std::string, std::string> m_attributes;
	public:
		LoadedValue(LoadedValue *parent, const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes = {});

		~LoadedValue();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &data) { m_value = data; }

		std::vector<LoadedValue *> &GetChildren() { return m_children; }

		LoadedValue *GetChild(const std::string &name, const bool &addIfNull = false, const bool &reportError = true);

		LoadedValue *GetChild(const unsigned int &index, const bool &addIfNull = false, const bool &reportError = true);

		LoadedValue *GetChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError = true);

	//	std::optional<LoadedValue *> SearchHierarchy(const std::vector<std::string> &names, const int &i = 0);

	//	std::optional<LoadedValue *> SearchHierarchyAttribs(const std::vector<std::string> &names, const std::string &childName, const std::string &attribute, const std::string &value, const int &i = 0);

		std::map<std::string, std::string> GetAttributes() const { return m_attributes; }

		std::string GetAttribute(const std::string &attribute) const;

		void AddAttribute(const std::string &attribute, const std::string &value);

		bool RemoveAttribute(const std::string &attribute);

		void AddChild(LoadedValue *value);

		template<typename T>
		void SetChild(const std::string &name, const T &value)
		{
			std::string strValue = std::to_string(value);
			auto child = GetChild(name);

			if (child == nullptr)
			{
				child = new LoadedValue(this, name, strValue);
				m_children.emplace_back(child);
				return;
			}

			child->m_value = strValue;
		}

		template<typename T>
		T Get()
		{
			std::string data = GetValue();
			return FormatString::ConvertTo<T>(data);
		}

		template<typename T>
		void Set(const T &data)
		{
			SetValue(std::to_string(data));
		}

		std::string GetString();

		void SetString(const std::string &data);

		static void PrintDebug(LoadedValue *value, const int &level = 0);
	};
}
