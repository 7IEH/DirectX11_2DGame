#include "pch.h"
#include "EHHierarchy.h"

#include "EHLevelMgr.h"
#include "EHInspector.h"

Hierarchy::Hierarchy()
	:UI("Hierarch","##Hierarchy")
{
}

Hierarchy::~Hierarchy()
{
}

/********************
| Object Picking
********************/

void Hierarchy::Render_Update()
{
	Level* _curLevel = LevelMgr::GetInst()->GetCurLevel();

	static int testidx = 0;

	for (size_t _idx = 0;_idx < (UINT)LAYER_TYPE::END;_idx++)
	{
		Layer* _layer = _curLevel->GetLayer(LAYER_TYPE(_idx));
		vector<GameObject*>& _objs = _layer->GetLayerObject();

		for (size_t _obj = 0;_obj < _objs.size();_obj++)
		{
			wstring _name_w = _objs[_obj]->GetName();
			string _name;

			bool _picking = _objs[_obj]->GetPicking();
			_name = EH::ConvertString(_name_w.c_str());

			ImGui::Selectable(_name.c_str(), &_picking);

			_objs[_obj]->SetPicking(_picking);
		}
	}

	bool enabled = FALSE;
	for (size_t _layer = 0; _layer < (UINT)LAYER_TYPE::END;_layer++)
	{
		Layer* _layers = _curLevel->GetLayer(LAYER_TYPE(_layer));
		vector<GameObject*>& _objs = _layers->GetLayerObject();

		for (size_t _obj = 0; _obj < _objs.size();_obj++)
		{
			if (_objs[_obj]->GetPicking() == TRUE)
			{
				m_Inspector->SetTargetObject(_objs[_obj]);
				enabled = TRUE;
				break;
			}
		}

		if (enabled)
			break;
	}
}