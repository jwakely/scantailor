/*
	Scan Tailor - Interactive post-processing tool for scanned pages.
	Copyright (C) 2007-2009  Joseph Artsimovich <joseph_a@mail.ru>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ZONE_INTERACTION_CONTEXT_H_
#define ZONE_INTERACTION_CONTEXT_H_

#include "EditableSpline.h"
#include "SplineVertex.h"
#include "EditableZoneSet.h"
#include <functional>

class InteractionHandler;
class InteractionState;
class ImageViewBase;
class EditableZoneSet;

class ZoneInteractionContext
{
public:
	typedef std::function<
		InteractionHandler* ()
	> DefaultInteractionCreator;

	typedef std::function<
		InteractionHandler* (InteractionState& interaction)
	> ZoneCreationInteractionCreator;

	typedef std::function<
		InteractionHandler* (
			InteractionState& interaction,
			EditableSpline::Ptr const& spline, SplineVertex::Ptr const& vertex
		)
	> VertexDragInteractionCreator;

	typedef std::function<
		InteractionHandler* (InteractionState& interaction)
	> ContextMenuInteractionCreator;

	typedef std::function<
		void (EditableZoneSet::Zone const& zone)
	> ShowPropertiesCommand;

	ZoneInteractionContext(ImageViewBase& image_view, EditableZoneSet& zones);

	virtual ~ZoneInteractionContext();

	ImageViewBase& imageView() { return m_rImageView; }

	EditableZoneSet& zones() { return m_rZones; }

	virtual InteractionHandler* createDefaultInteraction() {
		return m_defaultInteractionCreator();
	}

	void setDefaultInteractionCreator(DefaultInteractionCreator creator) {
		m_defaultInteractionCreator = std::move(creator);
	}

	virtual InteractionHandler* createZoneCreationInteraction(InteractionState& interaction) {
		return m_zoneCreationInteractionCreator(interaction);
	}

	void setZoneCreationInteractionCreator(ZoneCreationInteractionCreator creator) {
		m_zoneCreationInteractionCreator = std::move(creator);
	}

	virtual InteractionHandler* createVertexDragInteraction(
			InteractionState& interaction, EditableSpline::Ptr const& spline,
			SplineVertex::Ptr const& vertex) {
		return m_vertexDragInteractionCreator(interaction, spline, vertex);
	}

	void setVertexDragInteractionCreator(VertexDragInteractionCreator creator) {
		m_vertexDragInteractionCreator = std::move(creator);
	}

	/**
	 * \note This function may refuse to create a context menu interaction by returning null.
	 */
	virtual InteractionHandler* createContextMenuInteraction(InteractionState& interaction) {
		return m_contextMenuInteractionCreator(interaction);
	}

	void setContextMenuInteractionCreator(ContextMenuInteractionCreator creator) {
		m_contextMenuInteractionCreator = std::move(creator);
	}

	virtual void showPropertiesCommand(EditableZoneSet::Zone const& zone) {
		m_showPropertiesCommand(zone);
	}

	void setShowPropertiesCommand(ShowPropertiesCommand command) {
		m_showPropertiesCommand = std::move(command);
	}
private:
	/**
	 * Creates an instance of ZoneDefaultInteraction.
	 */
	InteractionHandler* createStdDefaultInteraction();

	/**
	 * Creates an instance of ZoneCreationInteraction.
	 */
	InteractionHandler* createStdZoneCreationInteraction(InteractionState& interaction);

	/**
	 * Creates an instance of ZoneVertexDragInteraction.
	 */
	InteractionHandler* createStdVertexDragInteraction(
		InteractionState& interaction, EditableSpline::Ptr const& spline,
		SplineVertex::Ptr const& vertex);

	/**
	 * Creates an instance of ZoneContextMenuInteraction.  May return null.
	 */
	InteractionHandler* createStdContextMenuInteraction(InteractionState& interaction);

	static void showPropertiesStub(EditableZoneSet::Zone const&) {}

	ImageViewBase& m_rImageView;
	EditableZoneSet& m_rZones;
	DefaultInteractionCreator m_defaultInteractionCreator;
	ZoneCreationInteractionCreator m_zoneCreationInteractionCreator;
	VertexDragInteractionCreator m_vertexDragInteractionCreator;
	ContextMenuInteractionCreator m_contextMenuInteractionCreator;
	ShowPropertiesCommand m_showPropertiesCommand;
};

#endif
