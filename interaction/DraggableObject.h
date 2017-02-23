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

#ifndef DRAGGABLE_OBJECT_H_
#define DRAGGABLE_OBJECT_H_

#include "InteractionState.h"
#include "Proximity.h"
#include <functional>

class ObjectDragHandler;
class QPoint;
class QPointF;
class QPainter;

class DraggableObject
{
public:
	typedef std::function<
		void (QPainter& painter, InteractionState const& interaction)
	> PaintCallback;

	typedef std::function<
		Proximity (InteractionState const& interaction)
	> ProximityThresholdCallback;

	typedef std::function<
		int ()
	> ProximityPriorityCallback;

	typedef std::function<
		Proximity (QPointF const& mouse_pos)
	> ProximityCallback;

	typedef std::function<
		void (QPointF const& mouse_pos)
	> DragInitiatedCallback;

	typedef std::function<
		void (QPointF const& mouse_pos)
	> DragContinuationCallback;

	typedef std::function<
		void (QPointF const& mouse_pos)
	> DragFinishedCallback;

	DraggableObject()
		: m_paintCallback(&DraggableObject::defaultPaint),
		m_proximityThresholdCallback(&DraggableObject::defaultProximityThreshold),
		m_proximityPriorityCallback(&DraggableObject::defaultProximityPriority),
		m_proximityCallback(),
		m_dragInitiatedCallback(),
		m_dragContinuationCallback(),
		m_dragFinishedCallback(&DraggableObject::defaultDragFinished)
	{}

	virtual ~DraggableObject() {}

	virtual void paint(QPainter& painter, InteractionState const& interaction) {
		m_paintCallback(painter, interaction);
	}

	void setPaintCallback(PaintCallback callback) {
		m_paintCallback = std::move(callback);
	}

	/**
	 * \return The maximum distance from the object (in widget coordinates) that
	 *         still allows to initiate a dragging operation.
	 */
	virtual Proximity proximityThreshold(InteractionState const& interaction) const {
		return m_proximityThresholdCallback(interaction);
	}

	void setProximityThresholdCallback(ProximityThresholdCallback callback) {
		m_proximityThresholdCallback = std::move(callback);
	}

	/**
	 * Sometimes a more distant object should be selected for dragging in favor of
	 * a closer one.  Consider for example a line segment with handles at its endpoints.
	 * In this example, you would assign higher priority to those handles.
	 */
	virtual int proximityPriority() const {
		return m_proximityPriorityCallback();
	}

	void setProximityPriorityCallback(ProximityPriorityCallback callback) {
		m_proximityPriorityCallback = std::move(callback);
	}

	/**
	 * \return The proximity from the mouse position in widget coordinates to
	 *         any draggable part of the object.
	 */
	virtual Proximity proximity(QPointF const& widget_mouse_pos) {
		return m_proximityCallback(widget_mouse_pos);
	}

	void setProximityCallback(ProximityCallback callback) {
		m_proximityCallback = std::move(callback);
	}

	/**
	 * \brief Called when dragging is initiated, that is when the mouse button is pressed.
	 */
	virtual void dragInitiated(QPointF const& mouse_pos) {
		m_dragInitiatedCallback(mouse_pos);
	}

	void setDragInitiatedCallback(DragInitiatedCallback callback) {
		m_dragInitiatedCallback = std::move(callback);
	}

	/**
	 * \brief Handles a request to move to a particular position in widget coordinates.
	 */
	virtual void dragContinuation(QPointF const& mouse_pos) {
		m_dragContinuationCallback(mouse_pos);
	}

	void setDragContinuationCallback(DragInitiatedCallback callback) {
		m_dragContinuationCallback = std::move(callback);
	}

	/**
	 * \brief Called when dragging is finished, that is when the mouse button is released.
	 */
	virtual void dragFinished(QPointF const& mouse_pos) {
		m_dragFinishedCallback(mouse_pos);
	}

	void setDragFinishedCallback(DragFinishedCallback callback) {
		m_dragFinishedCallback = std::move(callback);
	}
private:
	static void defaultPaint(QPainter&, InteractionState const&) {}

	static Proximity defaultProximityThreshold(InteractionState const& interaction) {
		return interaction.proximityThreshold();
	}

	static int defaultProximityPriority() { return 0; }

	static void defaultDragFinished(QPointF const&) {}

	PaintCallback m_paintCallback;
	ProximityThresholdCallback m_proximityThresholdCallback;
	ProximityPriorityCallback m_proximityPriorityCallback;
	ProximityCallback m_proximityCallback;
	DragInitiatedCallback m_dragInitiatedCallback;
	DragContinuationCallback m_dragContinuationCallback;
	DragFinishedCallback m_dragFinishedCallback;
};

#endif
