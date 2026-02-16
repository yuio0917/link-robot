#include "include/LinkRobot.hpp"

void LinkRobot::solveIK(std::vector<std::vector<float>> &charvec) {

    for (int i = 0; i < charvec[0].size(); i++){
        double dist_joint1_to_pen = sqrt((charvec[0][i])*(charvec[0][i]) + (charvec[1][i])*(charvec[1][i]));
        double dist_joint2_to_pen = sqrt((charvec[0][i] - joint2_x)*(charvec[0][i] - joint2_x) + (charvec[1][i])*(charvec[1][i]));

        double theta_13p = ((dist_joint1_to_pen)*(dist_joint1_to_pen) + l2*l2 - (l4 + l6)*(l4 + l6)) / (2*l2*dist_joint1_to_pen);
        theta_1 = std::acos(theta_13p) + atan2(charvec[1][i], charvec[0][i]);

        double joint3_x = l2*cos(theta_1);
        double joint3_y = l2*sin(theta_1);

        double dist_joint3_to_pen = l4 + l6;
        
        double unit_vec_x = (charvec[0][i] - joint3_x) / dist_joint3_to_pen;
        double unit_vec_y = (charvec[1][i] - joint3_y) / dist_joint3_to_pen;

        double joint5_x = joint3_x + l4*unit_vec_x;
        double joint5_y = joint3_y + l4*unit_vec_y;

        double dist_joint2_to_joint5 = sqrt((joint5_x - joint2_x)*(joint5_x - joint2_x) + (joint5_y - joint2_y)*(joint5_y - joint2_y));

        double theta_25p = ((dist_joint2_to_joint5)*(dist_joint2_to_joint5) + l3*l3 - l5*l5) / (2*l3*dist_joint2_to_joint5);
        theta_2 = atan2(joint5_y - joint2_y, joint5_x - joint2_x) - acos(theta_25p);
        
        
        angleVec.push_back(std::vector<double>{theta_1, theta_2});
    }
}