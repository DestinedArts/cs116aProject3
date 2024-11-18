//
//  Primitives.h - Simple 3D Primitives with with Hierarchical Transformations
//
//  
//  (c) Kevin M. Smith  - 24 September 2018
//

#include <cmath>
#include "ofApp.h"
#include "Primitives.h"

// Generate a rotation matrix that rotates v1 to v2
// v1, v2 must be normalized
//
glm::mat4 SceneObject::rotateToVector(glm::vec3 v1, glm::vec3 v2) {

	glm::vec3 w1 = glm::normalize(v1);
	glm::vec3 w2 = glm::normalize(v2);
	glm::vec3 axis = glm::normalize(glm::cross(w1, w2));
	glm::quat q = glm::angleAxis(glm::angle(w1, w2), axis);
	return glm::toMat4(q);
}


// Draw a Unit cube (size = 2) transformed 
//
void Cone::draw() {

	//rotateToVector(glm::vec3(0, 1, 0), glm::vec3(1, 1, 1));

	glm::mat4 m = getMatrix();

	//   push the current stack matrix and multiply by this object's
	//   matrix. now all vertices will be transformed by this matrix
	//
	ofPushMatrix();
	ofMultMatrix(m);
	ofDrawCone(radius, height);
	ofPopMatrix();


	// draw axis
	//
	ofApp::drawAxis(m, 1.5);
}

//  Cone::intersect - test intersection with bounding box.  Note that
//  intersection test is done in object space with an axis aligned box (AAB), 
//  the input ray is provided in world space, so we need to transform the ray to object space.
//  this method does NOT return a normal.
//
bool Cone::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {

	// transform Ray to object space.  
	//
	glm::mat4 mInv = glm::inverse(getMatrix());
	glm::vec4 p = mInv * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = mInv * glm::vec4(ray.p + ray.d, 1.0);
	glm::vec3 d = glm::normalize(p1 - p);


	// intesect method we use will be Willam's  (see box.h and box.cc for reference).
	// note that this class has it's own version of Ray, Vector3  (TBD: port to GLM)
	//
	_Ray boxRay = _Ray(Vector3(p.x, p.y, p.z), Vector3(d.x, d.y, d.z));

	// we will test for intersection in object space (object is a "unit" cube edge is len=2)
	//
	Box box = Box(Vector3(-radius, -radius, 0), Vector3(radius, radius, height));
	return (box.intersect(boxRay, -1000, 1000));


}

// Draw a Unit cube (size = 2) transformed 
//
void Cube::draw() {

    //   get the current transformation matrix for this object
	//
	glm::mat4 m = getMatrix();

	//   push the current stack matrix and multiply by this object's
	//   matrix. now all vertices dran will be transformed by this matrix
	//
	ofPushMatrix();
	ofMultMatrix(m);
	ofDrawBox(width, height, depth);
	ofPopMatrix();

	// draw axis
	//
	ofApp::drawAxis(m, 1.5);

}

void Sphere::draw() {

	//   get the current transformation matrix for this object
   //
	glm::mat4 m = getMatrix();

	//   push the current stack matrix and multiply by this object's
	//   matrix. now all vertices drawn will be transformed by this matrix
	//
	ofPushMatrix();
	ofMultMatrix(m);
	ofDrawSphere(radius);
	ofPopMatrix();

	// draw axis
	//
	ofApp::drawAxis(m, 1.5);
	
}

bool Sphere::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {

	// transform Ray to object space.  
	//
	glm::mat4 mInv = glm::inverse(getMatrix());
	glm::vec4 p = mInv * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = mInv * glm::vec4(ray.p + ray.d, 1.0);
	glm::vec3 d = glm::normalize(p1 - p);

	return (glm::intersectRaySphere(glm::vec3(p), d, glm::vec3(0, 0, 0), radius, point, normal));
}

//  Cube::intersect - test intersection with the unit Cube.  Note that
//  intersection test is done in object space with an axis aligned box (AAB), 
//  the input ray is provided in world space, so we need to transform the ray to object space.
//  this method does NOT return a normal.
//
bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {

	// transform Ray to object space.  
	//
	glm::mat4 mInv = glm::inverse(getMatrix());
	glm::vec4 p = mInv * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = mInv * glm::vec4(ray.p + ray.d, 1.0);
	glm::vec3 d = glm::normalize(p1 - p);


	// intesect method we use will be Willam's  (see box.h and box.cc for reference).
	// note that this class has it's own version of Ray, Vector3  (TBD: port to GLM)
	//
	_Ray boxRay = _Ray(Vector3(p.x, p.y, p.z), Vector3(d.x, d.y, d.z));

	// we will test for intersection in object space (object is a "unit" cube edge is len=2)
	//
	Box box = Box(Vector3(-width/2.0, -height/2.0, -depth/2.0), Vector3(width/2.0, height/2.0, depth/2.0));
	return (box.intersect(boxRay, -1000, 1000));

}

// Intersect Ray with Plane  (wrapper on glm::intersect*)
//
bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normalAtIntersect) {
	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normalAtIntersect = this->normal;
		glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
		glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);
		if (point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
			insidePlane = true;
		}
	}
	return insidePlane;
}

void Joint::draw()
{
	Sphere::draw();
	if (parent != nullptr)
	{
		double dist = sqrt(glm::dot(position, position)) - radius - ((Joint*)parent)->radius;
		if (dist > 0)
		{
			// if joints do not overlap, then draw a cone
			Cone cone(glm::vec3(0, dist + radius, 0), glm::vec3(0, 0, 0), glm::vec3(1, dist, 1), ofColor::hotPink);
			cone.parent = this;
			//cone.draw();
			//cout << "Pos " << position << "; PPos " << parent->position << "; Dist " << dist << endl; 
			//cout << "Delta " << delta << "; rotation " << rotation << endl;
		}

		glm::mat4 m = getMatrix();

		//   push the current stack matrix and multiply by this object's
		//   matrix. now all vertices will be transformed by this matrix
		//
		ofPushMatrix();
		ofMultMatrix(m);
		ofDrawLine(glm::vec3(0, 0, 0), -position);	// draw the joint, draw the connection to the point
		for (int i = 0; i < 10; i++)
		{
			double rand1 = (0.5 * (double)rand()) / RAND_MAX - 0.5;
			double rand2 = (0.5 * (double)rand()) / RAND_MAX - 0.5;
			double rand3 = (0.5 * (double)rand()) / RAND_MAX - 0.5;
			glm::vec3 eps(rand1, rand2, rand3);
			ofDrawLine(glm::vec3(0, 0, 0), eps-position);	// draw the joint, draw the connection to the point
		}
		ofPopMatrix();
	}
}