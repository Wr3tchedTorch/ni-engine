#include <NiEngine/Container.h>

#include <memory>
#include <utility>
#include <algorithm>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/HUDComponent.h>
#include <vector>

ni::Container::Container(sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns)
{
	gap_         = gap;
	margin_      = margin;
	vertical_    = vertical;
	max_columns_ = max_columns;
}

void ni::Container::AddComponent(std::unique_ptr<HUDComponent> component)
{
	hud_components_.push_back(std::move(component));
}

void ni::Container::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	float previous_width_count  = 0;
	float previous_height_count = 0;

	float tallest_cell_height = 0;

	int previous_row = 0;
	for (int i = 0; i < hud_components_.size(); ++i)
	{
		auto& component = hud_components_.at(i);

		int grid_x = i % max_columns_;
		int grid_y = i / max_columns_;

		if (previous_row != grid_y)
		{
			previous_row         = grid_y;
			previous_width_count = 0;

			previous_height_count += tallest_cell_height;
			tallest_cell_height = 0;
		}

		sf::Vector2f position = GetBounds().position;
		position.x += previous_width_count  + grid_x * gap_.x + margin_.x;
		position.y += previous_height_count + grid_y * gap_.y + margin_.y;

		component->GetBounds().position = position;

		component->Render(target, states, store);

		previous_width_count += component->GetBounds().size.x;
		tallest_cell_height   = std::max(tallest_cell_height, component->GetBounds().size.y);
	}
}