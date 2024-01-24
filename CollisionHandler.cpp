#include "CollisionHandler.h"
#include "BallObjectHandler.h"
#include <cstdlib>
#include <ctime>

CollisionHandler::CollisionHandler() : currTime(0.0f), deltaTime(0.0f), gravity(glm::vec3(0.0f, -9.81f, 0.0f) * 1.0f) {}//constructor


CollisionHandler::~CollisionHandler() //destructor
{
}

void CollisionHandler::calculateTime()
{
    //time calculation
	float newTime = glfwGetTime(); //current time
	deltaTime = newTime - currTime; //delta time for calculations
	currTime = newTime;
}

void CollisionHandler::integrate(Setters_Getters *S_G)  //numerical integration  to calculate the new position, velocity, angular position, and angular velocity
{
    //EULER METHOD - FIRST ORDER ODE
	glm::vec3 newVel = currVel + deltaTime*currAcc;  //V_ball = V0 + delta_t*a    
	glm::vec3 newPos = currPos + deltaTime*newVel;    // X_ball = X0 + v*t
	
	float newAngVel = currAngVel + deltaTime*currAngAcc;  //Omege_ball = Omega_0 + t*alpha
	float newAngPos = currAngPos + deltaTime*newAngVel; //D_ball = D_0 + t*omega

    // set the obtained values above
	S_G->setAcceleration(currAcc);
    S_G->setVelocity(newVel);
    S_G->setPosition(newPos);
    S_G->setAngularAcceleration(currAngAcc);
    S_G->setAngularVelocity(newAngVel);
    S_G->setAngularPosition(newAngPos);
}

void CollisionHandler::calculateMovement(vector<Setters_Getters*>* theEntityList, GLFWwindow* window) {
    for (auto entity : *theEntityList) { // Iterate through each entity in the list for motion calculation
        if (entity->getOtype() != 'W') { // calculation of motion expect wall, it is fixed

            // Getting properties of the entity for calculation
            currPos = entity->getPosition();
            currAcc = entity->getAcceleration();
            currVel = entity->getVelocity();
            currAcc = gravity; // Applying gravity, gravity applied thorough y axes and (-) direction
            currAngAcc = entity->getAngularAcceleration();
            currAngVel = entity->getAngularVelocity();
            currAngPos = entity->getAngularPosition();
            integrate(entity); //integration
        }
    }
}



// This function iterates over pairs of entities in the provided entity list to detect and handle collisions.
// It supports collisions between balls and between balls and walls, resolving them by updating positions, velocities,
// and angular velocities. The collision response is calculated based on the principles of physics, involving
// momentum conservation, elastic collisions, and reflection.

void CollisionHandler::resolveCollision(vector<Setters_Getters*> * theEntityList) {

    //Defining all variables which will be used calculations
    glm::vec3 iVel;
    glm::vec3 iPos;
    glm::vec3 jPos;
    glm::vec3 jVel;

    glm::vec3 posVector;
    float vLength;
    float move;

    glm::vec3 normal;
    glm::vec3 p1Normal;
    glm::vec3 p2Normal;
    glm::mat4 rotCoSystem1;
    glm::mat4 posCoSystem1;
    glm::mat4 rotCoSystem2;
    glm::mat4 posCoSystem2;

    glm::vec4 vec_1;
    glm::vec4 nVec_1;

    glm::vec4 vec_2;
    glm::vec4 nVec_2;

    glm::vec3 vec3_1;
    glm::vec3 vec3_2;
    glm::vec3 vec3_3;
    glm::vec3 vec3_4;

    glm::vec4 nBasePos;

    glm::vec2 Pdim;

   //nested loop for iterating over pairs of object
    for (int i = 0; i < theEntityList->size() - 1; i++) {
        iPos = theEntityList->at(i)->getPosition();
        iVel = theEntityList->at(i)->getVelocity();

        for (int j = i + 1; j < theEntityList->size(); j++) {

            jPos = theEntityList->at(j)->getPosition();
            jVel = theEntityList->at(j)->getVelocity();

            posVector = iPos - jPos;
            vLength = glm::length(posVector);

            //BALL TO BALL
            //if object type pairs is B which means Ball
            if (theEntityList->at(i)->getOtype() == 'B' && theEntityList->at(j)->getOtype() == 'B') {
                tempBall1 = static_cast<BallObjectHandler *> (theEntityList->at(i));
                tempBall2 = static_cast<BallObjectHandler *> (theEntityList->at(j));
                if (vLength < tempBall1->getRadius() + tempBall2->getRadius()) {

                    posVector = glm::normalize(posVector);// Calculatiion normalized collision normal

                    // Obtaining masses and calculate relative move
                    m1 = theEntityList->at(i)->getMass();
                    m2 = theEntityList->at(j)->getMass();

                    move = (tempBall1->getRadius() + tempBall2->getRadius() - vLength);

                    move /= 2.0f;
                    // Calculating collision normal and perpendicular vectors
                    normal = posVector;
                    p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
                    p2Normal = glm::cross(normal, p1Normal);
                    rotCoSystem1 = glm::mat4(glm::vec4(glm::normalize(normal), 0.0f),
                                             glm::vec4(glm::normalize(p1Normal), 0.0f),
                                             glm::vec4(glm::normalize(p2Normal), 0.0f),
                                             glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                    // Transforming velocities into collision coordinate system
                    vec_1 = glm::vec4(iVel, 1.0f);
                    nVec_1 = glm::transpose(rotCoSystem1) * vec_1;

                    vec_2 = glm::vec4(jVel, 1.0f);
                    nVec_2 = glm::transpose(rotCoSystem1) * vec_2;


                    // In here, to make simulation faster, we neglect rotational momentum at the time of the collision. So, translational momentum is not affected from rotational momentum
                    // Elastic Collision Formulas:
                    // v3 and v4 represent the final velocities of the two colliding balls after the collision.
                    // m1 and m2 are the masses of the two balls.
                    // nVec_1.x and nVec_2.x are the components of the velocity vectors along the line of collision.
                    // This formula considers the masses and initial velocities of both balls, as well as the elasticity factor.
                    v3 = (m1 * nVec_1.x + m2 * nVec_2.x + m2 * elasticity * (nVec_2.x - nVec_1.x)) / (m1 + m2); //elastic collision formula if elasticity is 1, it is elastic collision
                    v4 = (m1 * nVec_1.x + m2 * nVec_2.x - m1 * elasticity * (nVec_2.x - nVec_1.x)) / (m1 + m2);

                    // Transforming velocities back to world coordinate system
                    nVec_1 = glm::vec4(v3, nVec_1.y, nVec_1.z, 1.0f);
                    vec_1 = rotCoSystem1 * nVec_1;
                    theEntityList->at(i)->setVelocity(glm::vec3(vec_1));

                    nVec_2 = glm::vec4(v4, nVec_2.y, nVec_2.z, 1.0f);

                    vec_2 = rotCoSystem1 * nVec_2;
                    theEntityList->at(j)->setVelocity(glm::vec3(vec_2));

                    // Updating positions after collision for firs ball
                    nBasePos = glm::transpose(rotCoSystem1) * glm::vec4(iPos, 1.0f);
                    nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
                    nBasePos = rotCoSystem1 * nBasePos;
                    theEntityList->at(i)->setPosition(glm::vec3(nBasePos));

                    // Updating positions after collision for second ball
                    nBasePos = glm::transpose(rotCoSystem1) * glm::vec4(jPos, 1.0f);
                    nBasePos = glm::vec4(nBasePos.x - move, nBasePos.y, nBasePos.z, 1.0f);
                    nBasePos = rotCoSystem1 * nBasePos;
                    theEntityList->at(j)->setPosition(glm::vec3(nBasePos));


                    // ROTATION
                    // Updating angular velocities and rotation axes after collision
                    vec3_1 = tempBall1->getAngularVelocity() * glm::normalize(glm::cross(posVector, tempBall1->getRotAxis())) - glm::vec3(0.0f, nVec_2.y, nVec_2.z);  //Angular velocity = Linear velocity x Position vector in world coordinate
                    vec3_2 = tempBall2->getAngularVelocity() * glm::normalize(glm::cross(posVector, tempBall2->getRotAxis())) + glm::vec3(0.0f, nVec_1.y, nVec_1.z);  //vectorel multiplication (cross product)
                    theEntityList->at(i)->setAngularVelocity(glm::length(vec3_1));
                    theEntityList->at(j)->setAngularVelocity(glm::length(vec3_2));

                    vec3_1 = glm::cross(vec3_1, posVector);
                    vec3_2 = glm::cross(vec3_2, posVector);

                    theEntityList->at(i)->setRotAxis(vec3_1);
                    theEntityList->at(j)->setRotAxis(vec3_2);

                }
            }

            //BALL TO WALL
            //if the type of the objects are W (Wall) and B (Ball)
            if (theEntityList->at(i)->getOtype() == 'W' && theEntityList->at(j)->getOtype() == 'B') {
                // Casting the objects to their respective types for access to specific properties
                tempWall = static_cast<WallObjectHandler *> (theEntityList->at(i));
                tempBall1 = static_cast<BallObjectHandler *> (theEntityList->at(j));

                // Calculating the local coordinate system of the wall
                normal = tempWall->getNormal();
                p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
                p2Normal = glm::cross(normal, p1Normal);
                rotCoSystem1 = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f),
                                         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

                // Getting the dimensions and position of the wall
                Pdim = tempWall->getDim();
                posVector = tempWall->getPosition();


                // Transforming the position of the ball to the local coordinate system of the wall
                nBasePos = glm::transpose(rotCoSystem1) *
                           glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 0.0f,
                                     -posVector.x, -posVector.y, -posVector.z, 1.0f) * glm::vec4(jPos, 1.0f);

                // Check if the ball collides with the wall within its dimensions
                if (nBasePos.x > -tempBall1->getRadius() && nBasePos.x < tempBall1->getRadius() &&
                    nBasePos.y < Pdim.y / 2.0f && nBasePos.y > -Pdim.y / 2.0f && nBasePos.z < Pdim.x / 2.0f &&
                    nBasePos.z > -Pdim.x / 2.0f) {

                    // Reflecting the velocity of the ball based on the wall's normal
                    vec_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
                    nVec_2 = glm::inverse(rotCoSystem1) * vec_2;
                    nVec_2 = glm::vec4(nVec_2.x * 0.5f, nVec_2.y, nVec_2.z, 1.0f);
                    vec_2 = rotCoSystem1 * nVec_2;
                    theEntityList->at(j)->setVelocity(glm::vec3(vec_2));

                    // The calculated move value is determined based on the point of contact between the ball and the wall.
                    // The move value is associated with the balls radius and the distance of the contact point from the wall.
                    // If nBasePos.x (the x-coordinate of the contact point) is positive, it means the ball hit the right side, and move is positive.
                    // If nBasePos.x is negative, it means the ball hit the left side, and move is negative.
                    // We calculated  the move value by subtracting the distance of the contact point from the ball's radius.
                    // This ensures the ball does not go through the wall
                    // Then, the nBasePos.x value is updated with the addition of the move, allowing the contact point of the ball to slide.
                    // After this update, nBasePos represents the ball's new position.
                    move = (nBasePos.x / abs(nBasePos.x)) * (tempBall1->getRadius() - abs(nBasePos.x));
                    nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);

                    // Transforming the position back to the global coordinate system
                    nBasePos =
                            glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                      0.0f, 1.0f, 0.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f, 0.0f,
                                      posVector.x, posVector.y, posVector.z, 1.0f) *
                            (rotCoSystem1) *
                            nBasePos;
                    theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
                }
            }
        }
    }

}