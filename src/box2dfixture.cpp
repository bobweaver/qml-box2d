/*
 * box2dfixture.cpp
 * Copyright (c) 2010-2011 Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright (c) 2011 Daker Fernandes Pinheiro <daker.pinheiro@openbossa.org>
 * Copyright (c) 2011 Alessandro Portale <alessandro.portale@nokia.com>
 *
 * This file is part of the Box2D QML plugin.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "box2dfixture.h"
#include "box2dworld.h"
#include <QDebug>
#include "Common/b2Math.h"

/*!
\class Box2DFixture
*/
Box2DFixture::Box2DFixture(QQuickItem *parent) :
    QQuickItem(parent),
    mFixture(0),
    mFixtureDef(),
    factorWidth(1.0),
    factorHeight(1.0)
{
}
/*!
    \qmltype Fixture
    \instantiates Box2DFixture
    \inqmlmodule Box2D 1.1
    \brief Recall that shapes don’t know about {Body}{bodies} and may be used independently

of the physics simulation.
Therefore Box2D provides the Fixture  to attach shapes to {Body}{bodies}. A Body may have
zero or more {Body::fixtures}  {fixtures}.

A body with multiple fixtures is sometimes called a compound body.
Fixtures hold the following:

\list
\li  a single shape
\li  broad-phase proxies
\li density, friction, and restitution
\li collision filtering flags
\li back pointer to the parent body
\li user data
\li sensor flag
\endlist

These are described in the following sections.


\section1 Fixture Creation
Fixtures are created by initializing a fixture definition and then passing the definition to the
parent Body.

\code
add example

\endcode

This creates the fixture and attaches it to the Body. You can create multiple
fixtures on a single body. You can destroy a fixture on the parent Body.
You may do this to model a breakable object. Otherwise you can just leave the fixture alone
and let the body destruction take care of destroying the attached

\code
add destory example
ixtures. myBody->DestroyFixture(myFixture);
\endcode


\section1 Filtering

#FIXME
Collision filtering allows you to prevent collision between fixtures. For example, say you make a
character that rides a bicycle. You want the bicycle to collide with the terrain and the character to
collide with the terrain, but you don't want the character to collide with the bicycle (because they must
overlap). Box2D supports such collision filtering using categories and groups.
Box2D supports 16 collision categories. For each fixture you can specify which category it belongs to.
You also specify what other categories this fixture can collide with. For example, you could specify in a
multiplayer game that all players don't collide with each other and monsters don't collide with each
other, but players and monsters should collide. This is done with masking bits. For example:
playerFixtureDef.filter.categoryBits = 0x0002;
monsterFixtureDef.filter.categoryBits = 0x0004;
playerFixtureDef.filter.maskBits = 0x0004;
monsterFixtureDef.filter.maskBits = 0x0002;
Here is the rule for a collision to occur:
uint16 catA = fixtureA.filter.categoryBits;
uint16 maskA = fixtureA.filter.maskBits;
uint16 catB = fixtureB.filter.categoryBits;
uint16 maskB = fixtureB.filter.maskBits;

if ((catA & maskB) != 0 && (catB & maskA) != 0)
{
 // fixtures can collide
}
Collision groups let you specify an integral group index. You can have all fixtures with the same group
index always collide (positive index) or never collide (negative index). Group indices are usually used for
things that are somehow related, like the parts of a bicycle. In the following example, fixture1 and
fixture2 always collide, but fixture3 and fixture4 never collide.
fixture1Def.filter.groupIndex = 2;
fixture2Def.filter.groupIndex = 2;
fixture3Def.filter.groupIndex = -8;
fixture4Def.filter.groupIndex = -8;
Collisions between fixtures of different group indices are filtered according the category and mask bits.
In other words, group filtering has higher precedence than category filtering.
Note that additional collision filtering occurs in Box2D. Here is a list:  A fixture on a static body can only collide with a dynamic body.
 A fixture on a kinematic body can only collide with a dynamic body.
 Fixtures on the same body never collide with each other.
 You can optionally enable/disable collision between fixtures on bodies connected by a joint.
Sometimes you might need to change collision filtering after a fixture has already been created. You can
get and set the b2Filter structure on an existing fixture using b2Fixture::GetFilterData and
b2Fixture::SetFilterData. Note that changing the filter data will not add or remove contacts until the next
time step (see the World class).

see {filtering}{Filtering Example}

    */



/*!
/qmlproperty float Fixture::density
DOCME

Density
The fixture density is used to compute the mass properties of the parent body. The density can be zero
or positive. You should generally use similar densities for all your fixtures. This will improve stacking
stability.

The mass of a body is not adjusted when you set the density. You must call ResetMassData for this to
occur.
\code
fixture->SetDensity(5.0f);
body->ResetMassData();
\endcode

*/
float Box2DFixture::density() const
{
    return mFixtureDef.density;
}

void Box2DFixture::setDensity(float density)
{
    if (mFixtureDef.density == density)
        return;

    mFixtureDef.density = density;
    if (mFixture)
        mFixture->SetDensity(density);
    emit densityChanged();
}


/*!
\qmlproperty float Fixture::friction
DOCME
friction is used to make objects slide along each other realistically. Box2D supports

\list
\li Body.Static
\li Body.Dynamic
\endlist

friction, but uses the same parameter for both. Friction is simulated accurately in Box2D and
 the friction strength is proportional to the normal force (this is called Coulomb friction).
The friction parameter is usually set between 0 and 1, but can be any non-negative value.
 A friction value of 0 turns off friction and a value of 1 makes the friction strong.
When the friction force is computed between two shapes, Box2D must combine the friction
parameters of the two parent fixtures. This is done with the geometric mean:

\code
float32 friction;
friction = sqrtf(fixtureA->friction * fixtureB->friction);
\endcode

So if one fixture has zero friction then the contact will have zero friction.
You can override the default mixed friction using b2Contact::SetFriction. This is usually done in the
b2ContactListener callback.



*/
float Box2DFixture::friction() const
{
    return mFixtureDef.friction;
}

void Box2DFixture::setFriction(float friction)
{
    if (mFixtureDef.friction == friction)
        return;

    mFixtureDef.friction = friction;
    if (mFixture)
        mFixture->SetFriction(friction);
    emit frictionChanged();
}

/*!
\qmlproperty float Fixture::restitution
DOCME

restitution is used to make objects bounce. The restitution value is usually set to be
 between 0 and 1.

Consider dropping a ball on a table. A value of zero means the ball won't bounce.
This is called an inelastic collision. A value of one means the ball's velocity will be exactly
 reflected. This is called a perfectly elastic collision. Restitution is combined using the following
 formula.

#FIXME
\code
float32 restitution;
restitution = b2Max(fixtureA->restitution, fixtureB->restitution);
\endcode

restitution is combined this way so that you can have a bouncy super ball without having a
bouncy floor.

You can override the default mixed restitution using b2Contact::SetRestitution. This is usually
done in the b2ContactListener callback. When a shape develops multiple contacts, restitution is
 simulated approximately. This is because Box2D uses an iterative solver. Box2D also uses
 inelastic collisions when the collision velocity is small. This is done to prevent jitter.

See b2_velocityThreshold in b2Settings.h.
*/
float Box2DFixture::restitution() const
{
    return mFixtureDef.restitution;
}

void Box2DFixture::setRestitution(float restitution)
{
    if (mFixtureDef.restitution == restitution)
        return;

    mFixtureDef.restitution = restitution;
    if (mFixture)
        mFixture->SetRestitution(restitution);
    emit restitutionChanged();
}

bool Box2DFixture::isSensor() const
{
    return mFixtureDef.isSensor;
}

/*!
\qmlproperty bool Fixture::sensor
where is the getter function ?
*/
void Box2DFixture::setSensor(bool sensor)
{
    if (mFixtureDef.isSensor == sensor)
        return;

    mFixtureDef.isSensor = sensor;
    if (mFixture)
        mFixture->SetSensor(sensor);
    emit sensorChanged();
}

/*!
\qmlproperty enum Fixture::categories
DOCME
*/
Box2DFixture::CategoryFlags Box2DFixture::categories() const
{
    return CategoryFlags(mFixtureDef.filter.categoryBits);
}

void Box2DFixture::setCategories(CategoryFlags layers)
{
    if (mFixtureDef.filter.categoryBits == layers)
        return;

    mFixtureDef.filter.categoryBits = layers;
    emit categoriesChanged();
}

/*!
\qmlproperty enum Fixture::collidesWith
DOCME
*/
Box2DFixture::CategoryFlags Box2DFixture::collidesWith() const
{
    return CategoryFlags(mFixtureDef.filter.maskBits);
}

void Box2DFixture::setCollidesWith(CategoryFlags layers)
{
    if (mFixtureDef.filter.maskBits == layers)
        return;

    mFixtureDef.filter.maskBits = layers;
    emit collidesWithChanged();
}

/*!
/qmlproperty int Fixture::groupIndex
DOCME
*/
int Box2DFixture::groupIndex() const
{
    return mFixtureDef.filter.groupIndex;
}

void Box2DFixture::setGroupIndex(int groupIndex)
{
    if (mFixtureDef.filter.groupIndex == groupIndex)
        return;

    mFixtureDef.filter.groupIndex = groupIndex;
    emit groupIndexChanged();
}

void Box2DFixture::createFixture(b2Body *body)
{
    b2Shape *shape = createShape();
    if (!shape)
        return;

    mFixtureDef.shape = shape;
    mFixture = body->CreateFixture(&mFixtureDef);
    mFixture->SetUserData(this);
    mBody = body;
    delete shape;
}

/*!
\qmlsignal Fixture::GetBody()
DOCME
*/
Box2DBody *Box2DFixture::GetBody() const
{
    return static_cast<Box2DBody *>(mBody->GetUserData());
}

void Box2DFixture::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if(!isComponentComplete()) return;

    qreal nw = newGeometry.width();
    qreal nh = newGeometry.height();
    qreal ow = oldGeometry.width();
    qreal oh = oldGeometry.height();

    if( (nw != ow && !qFuzzyCompare(ow,0.0)) ||  (nh != oh && !qFuzzyCompare(oh,0.0)) )
    {
        factorWidth = nw / ow;
        factorHeight = nh / oh;
        scale();
    }
}

void Box2DFixture::emitBeginContact(Box2DFixture *other)
{
    emit beginContact(other);
}

void Box2DFixture::emitContactChanged(Box2DFixture *other)
{
    emit contactChanged(other);
}

void Box2DFixture::emitEndContact(Box2DFixture *other)
{
    emit endContact(other);
}

void Box2DFixture::applyShape(b2Shape *shape)
{
    if(mFixture) mBody->DestroyFixture(mFixture);
    mFixtureDef.shape = shape;
    mFixture = mBody->CreateFixture(&mFixtureDef);
    mFixture->SetUserData(this);
    delete shape;
}

b2Vec2 *Box2DVerticesShape::scaleVertices()
{
    const int count = mVertices.length();
    b2Vec2 * vertices = new b2Vec2[count];
    for (int i = 0; i < count; ++i) {

        QPointF point = mVertices.at(i).toPointF();
        point.setX(point.x() * factorWidth);
        point.setY(point.y() * factorHeight);
        mVertices.replace(i,point);
        vertices[i].Set(point.x() / scaleRatio, -point.y() / scaleRatio);
    }
    return vertices;
}

//=================== BOX =======================

//QMLDOCS
/*!
    \qmltype Box
    \instantiates Box2DBox
    \inqmlmodule Box2D 1.1
    \brief Provids a Box to wrap fictures in.
*/
//C++ DOCS
/*!
\class Box2DBox
*/

b2Shape *Box2DBox::createShape()
{
    const qreal _x = x() / scaleRatio;
    const qreal _y = -y() / scaleRatio;
    const qreal _width = width() / scaleRatio;
    const qreal _height = height() / scaleRatio;

    vertices[0].Set(_x, _y);
    vertices[1].Set(_x , _y - _height);
    vertices[2].Set(_x + _width , _y - _height);
    vertices[3].Set(_x + _width , _y );
    for(int i = 1;i < 4;i ++)
    {
        if(i > 0)
        {
            if(b2DistanceSquared(vertices[i - 1], vertices[i]) <= b2_linearSlop * b2_linearSlop)
            {
                qWarning() << "Box: vertices are too close together";
                return 0;
            }
        }
    }

    int32 count = 4;
    b2PolygonShape *shape = new b2PolygonShape;
    shape->Set(vertices, count);
    return shape;
}

void Box2DBox::scale()
{
    if(mFixture)
    {
        b2Shape * shape = createShape();
        applyShape(shape);
    }
}

//=================== CIRCLE =======================

/*!
    \qmltype Circle
    \instantiates Box2DCircle
    \inqmlmodule Box2D 1.1
    \brief Circle Shapes
    Circle shapes have a position and radius. Circles are solid. You cannot make a hollow circle
    using the
\code
        Body{
        fixture{
        Circle{
            radius: 180;
            ...
            .......
            ............
        }
\endcode

*/

/*!
\qmlproperty int Circle::radius
    The over all radius of the Circle that is attached to the Fixture
     See also Rectangle::radius
*/

/*!
\class Box2DCircle
*/
b2Shape *Box2DCircle::createShape()
{
    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = mRadius / scaleRatio;
    shape->m_p.Set(shape->m_radius, -shape->m_radius);
    if(height() == 0 || width() == 0) {
        this->setWidth(shape->m_radius);
        this->setHeight(shape->m_radius);
    }
    return shape;
}

void Box2DCircle::scale()
{
    if(mFixture)
    {
        b2Shape * shape = createShape();
        applyShape(shape);
    }
}

//=================== POLYGON =======================

/*!
    \qmltype Polygon
    \instantiates Box2DPolygon
    \inqmlmodule Box2D 1.1
    \brief Polygon shapes are solid convex polygons.

        A polygon is convex when all line segments connecting two points in the interior do
not cross any edge of the polygon. Polygons are solid and never hollow. A polygon must have
 3 or more vertices

\image polygonConvertConcave.png

Polygons vertices are stored with a counter clockwise winding (CCW). We must be careful
 because the notion of CCW is with respect to a right-handed coordinate system with the
 z-axis pointing out of the plane. This might turn out to be clockwise on your screen,
depending on your coordinate system conventions.

\image CCWwinding.png

##FIXME TO BE MORE QML
You can create a polygon shape by passing in a vertex array.
The maximal size of the array is controlled by maxPolygonVertices which has a default
value of 8. This is sufficient to describe most convex polygons.
PolygonShape Set function automatically computes the convex hull and establishes the proper
winding order. This function is fast when the number of vertices is low. If you increase



\image polygonSkin.png

The polygon skin helps prevent tunneling by keeping the polygons separated.
This results in small gaps between the shapes.

Your visual representation can be larger than the polygon to hide any gaps.

\image polygonColideSkin.png


*/

/*!
\class Box2DPolygon
*/
b2Shape *Box2DPolygon::createShape()
{
    const int count = mVertices.length();
    if (count < 2 || count > b2_maxPolygonVertices) {
        qWarning() << "Polygon: Invalid number of vertices:" << count;
        return 0;
    }

    b2Vec2 *vertices = new b2Vec2[count];
    for (int i = 0; i < count; ++i) {
        const QPointF &point = mVertices.at(i).toPointF();
        vertices[i].Set(point.x() / scaleRatio, -point.y() / scaleRatio);
        if(i > 0)
        {
            if(b2DistanceSquared(vertices[i - 1], vertices[i]) <= b2_linearSlop * b2_linearSlop)
            {
                qWarning() << "Polygon: vertices are too close together";
                return 0;
            }
        }
    }

    b2PolygonShape *shape = new b2PolygonShape;
    shape->Set(vertices, count);
    delete[] vertices;
    return shape;
}

void Box2DPolygon::scale()
{
    if(mFixture)
    {
        b2Vec2 *vertices = scaleVertices();
        b2PolygonShape *shape = new b2PolygonShape;
        shape->Set(vertices, mVertices.count());
        delete[] vertices;
        applyShape(shape);
    }

}

//=================== CHAIN =======================



/*!
    \qmltype Chain
    \instantiates Box2DChain
    \inqmlmodule Box2D 1.1
    \brief The chain shape provides an efficient way to connect many edges together
to construct your static game worlds. Chain shapes automatically eliminate ghost collisions
 and provide two-sided collision.

\image chain1.png


##FIXME
// This a chain shape with isolated vertices
\code
b2Vec2 vs[4];
vs[0].Set(1.7f, 0.0f);
vs[1].Set(1.0f, 0.25f);
vs[2].Set(0.0f, 0.0f);
vs[3].Set(-1.7f, 0.4f);
ChainShape chain;
chain.CreateChain(vs, 4);
\endcode

You may have a scrolling game world and would like to connect several chains together.
 You can connect chains together using ghost vertices, like we did with EdgeShape.
\code
// Install ghost vertices
chain.SetPrevVertex(b2Vec2(3.0f, 1.0f));
chain.SetNextVertex(b2Vec2(-2.0f, 0.0f));
\endcode
You may also create loops automatically.
\code
// Create a loop. The first and last vertices are connected.
b2ChainShape chain;
chain.CreateLoop(vs, 4);
\endcode

Self-intersection of chain shapes is not supported. It might work, it might not.
 The code that prevents ghost collisions assumes there are no self-intersections of the chain.
 Also, very close vertices can cause problems. Make sure all your edges are longer than
linearSlop (5mm).

\image chain2.png

Each Edge in the chain is treated as a child shape and can be accessed by index.
 When a chain shape is connected to a Body, each Edge gets its own bounding box in the
 broad-phase collision tree.
\code
// Visit each child edge.
for (int32 i = 0; i < chain.GetChildCount(); ++i)
{
b2EdgeShape edge;
chain.GetChildEdge(&edge, i);
…
}
\endcode

*/
/*!
\qmlproperty bool Chain::loop
DOCME
*/
    /*!
    \qmlproperty QPointF Chain::prevVertex
    DOCME
    */
    /*!
    \qmlproperty QPointF Chain::nextVertex
    DOCME
    */

/*!
\class  Box2DChain
*/
b2Shape *Box2DChain::createShape()
{
    const int count = mVertices.length();
    if (count < 2) {
        qWarning() << "Chain: Invalid number of vertices:" << count;
        return 0;
    }

    b2Vec2 *vertices = new b2Vec2[count];
    for (int i = 0; i < count; ++i) {
        const QPointF &point = mVertices.at(i).toPointF();
        vertices[i].Set(point.x() / scaleRatio, -point.y() / scaleRatio);
        if(i > 0) {
            if(b2DistanceSquared(vertices[i - 1], vertices[i]) <= b2_linearSlop * b2_linearSlop)
            {
                qWarning() << "Chain: vertices are too close together";
                return 0;
            }
        }
    }

    b2ChainShape *shape = new b2ChainShape;
    if(mLoop) shape->CreateLoop(vertices, count);
    else shape->CreateChain(vertices, count);
    if(prevVertexFlag) shape->SetPrevVertex(b2Vec2(mPrevVertex.x() / scaleRatio,mPrevVertex.y() / scaleRatio));
    if(nextVertexFlag) shape->SetNextVertex(b2Vec2(mNextVertex.x() / scaleRatio,mNextVertex.y() / scaleRatio));
    delete[] vertices;
    return shape;
}

void Box2DChain::scale()
{
    if(mFixture)
    {
        b2Vec2 *vertices = scaleVertices();
        b2ChainShape *shape = new b2ChainShape;
        if(mLoop) shape->CreateLoop(vertices, mVertices.count());
        else shape->CreateChain(vertices, mVertices.count());
        delete[] vertices;
        applyShape(shape);
    }
}

//=================== EDGE =======================


/*!
    \qmltype Edge
    \instantiates Box2DEdge
    \inqmlmodule Box2D 1.1
    \brief Edge shapes are line segments.

These are provided to assist in making a free-form static environment for your game.
 A major limitation of edge shapes is that they can collide with circles and polygons but
not with themselves. The collision algorithms used by Box2D require that at least one
of two colliding shapes have volume. Edge shapes have no volume, so edge-edge collision
 is not possible.
\code
// This an edge shape.
b2Vec2 v1(0.0f, 0.0f);
b2Vec2 v2(1.0f, 0.0f);

b2EdgeShape edge;
edge.Set(v1, v2);
\endcode

In many cases a game environment is constructed by connecting several Edge shapes
 end-to-end. This can give rise to an unexpected artifact when a Polygon slides along the Chain
 of Edges. In the figure below we see a Box colliding with an internal vertex. These ghost
 collisions are caused when the Polygon collides with an internal vertex generating an
internal collision normal.


\image edge1.png

If edge1 did not exist this collision would seem fine. With edge1 present, the internal
 collision seems like a bug. But normally when Box2D collides two shapes (bodies), it views
 them in isolation. Fortunately, the Edge shape provides a mechanism for eliminating ghost
collisions by storing the adjacent ghost vertices. Box2D uses these ghost vertices to prevent
 internal collisions.

\image edge2.png

\code
// This is an edge shape with ghost vertices.
b2Vec2 v0(1.7f, 0.0f);
b2Vec2 v1(1.0f, 0.25f);
b2Vec2 v2(0.0f, 0.0f);
b2Vec2 v3(-1.7f, 0.4f);

b2EdgeShape edge;
edge.Set(v1, v2);
edge.m_hasVertex0 = true;
edge.m_hasVertex3 = true;
edge.m_vertex0 = v0;
edge.m_vertex3 = v3;
\endcode

In general stitching edges together this way is a bit wasteful and tedious.
see {Chain} {chain shapes}.


*/
/*!
\class  Box2DEdge
*/
b2Shape *Box2DEdge::createShape()
{
    const int count = mVertices.length();
    if (count != 2) {
        qWarning() << "Edge: Invalid number of vertices:" << count;
        return 0;
    }
    QPointF point1 = mVertices.at(0).toPointF();
    QPointF point2 = mVertices.at(1).toPointF();
    b2Vec2 vertex1(point1.x() / scaleRatio, -point1.y() / scaleRatio);
    b2Vec2 vertex2(point2.x() / scaleRatio, -point2.y() / scaleRatio);
    if(b2DistanceSquared(vertex1, vertex2) <= b2_linearSlop * b2_linearSlop)
    {
        qWarning() << "Edge: vertices are too close together";
        return 0;
    }
    b2EdgeShape *shape = new b2EdgeShape;
    shape->Set(vertex1,vertex2);

    return shape;
}

void Box2DEdge::scale()
{
    if(mFixture)
    {
        b2Vec2 *vertices = scaleVertices();
        b2EdgeShape *shape = new b2EdgeShape;
        shape->Set(vertices[0],vertices[1]);
        delete[] vertices;
        applyShape(shape);
    }
}
