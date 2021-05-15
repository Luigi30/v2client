#include "HudTargeting.h"

void HudTargeting::DrawTargetBrackets(SimulationObject *target)
{
	Types3D::Rectangle bound = target->GetModel()->Get2DBoundingBox(&target->transformation);
	GlidePrimitives::DrawLine(bound.x1, bound.y1, bound.x1+20, bound.y1, 0xFF00E000);
	GlidePrimitives::DrawLine(bound.x1, bound.y1, bound.x1, bound.y1+20, 0xFF00E000);
	GlidePrimitives::DrawLine(bound.x2, bound.y2, bound.x2-20, bound.y2, 0xFF00E000);
	GlidePrimitives::DrawLine(bound.x2, bound.y2, bound.x2, bound.y2-20, 0xFF00E000);

	GlidePrimitives::DrawLine(bound.x2, bound.y1, bound.x2-20, bound.y1, 0xFF00E000);
	GlidePrimitives::DrawLine(bound.x2, bound.y1, bound.x2, bound.y1+20, 0xFF00E000);
	GlidePrimitives::DrawLine(bound.x1, bound.y2, bound.x1+20, bound.y2, 0xFF00E000);
	GlidePrimitives::DrawLine(bound.x1, bound.y2, bound.x1, bound.y2-20, 0xFF00E000);
}