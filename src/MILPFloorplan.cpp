
#include "MILPFloorplan.h"

using namespace std;

void MILPFloorplan::milp_floorplanning(vector<int>& w, vector<int>& h, int timeout) {
    try {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        string log_file = "./log/milp-" + to_string(numBlocks) + "b-" + to_string(timeout) + "s-verbose.log";
        env.set("LogFile", log_file);
        env.set("Seed", "0");
        if (timeout != 0) env.set("TimeLimit", to_string(timeout));
        env.start();
        log_file = "./log/milp-" + to_string(numBlocks) + "b-" + to_string(timeout) + "s.log";
        ofstream log_verbose(log_file);
        if (log_verbose.fail()) {
            cout << "[ERROR] File doesn't exist!!" << endl;
            exit(1);
        }

        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        GRBVar Y = model.addVar(0.0, INFINITY, 0.0, GRB_INTEGER, "Y");
        vector<GRBVar> x(numBlocks), y(numBlocks), rotate(numBlocks);
        vector<vector<GRBVar> > xx, yy;
        for (int i = 0; i < numBlocks; i++) {
            x[i] = model.addVar(0.0, INFINITY, 0.0, GRB_INTEGER, "x" + to_string(i));
            y[i] = model.addVar(0.0, INFINITY, 0.0, GRB_INTEGER, "y" + to_string(i));
            rotate[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "rotate" + to_string(i));
        }
        for (int i = 0; i < numBlocks - 1; i++) {
            vector<GRBVar> tempx, tempy;
            for (int j = i + 1; j < numBlocks; j++) {
                tempx.push_back(model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "xx"+to_string(i)+"_"+to_string(j)));
                tempy.push_back(model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "yy"+to_string(i)+"_"+to_string(j)));
            }
            xx.push_back(tempx);
            yy.push_back(tempy);
        }

        // Set objective: Min Y
        model.setObjective(Y + 0, GRB_MINIMIZE);

        // Add constraints: within boundary
        for (int i = 0; i < numBlocks; i++) {
            // along the X-coordinate
            model.addConstr(x[i] >= 0);
            model.addConstr(x[i] + rotate[i]*h[i] + (1-rotate[i])*w[i] <= chipwidth);
            // along the Y-coordinate
            model.addConstr(y[i] >= 0);
            model.addConstr(y[i] + rotate[i]*w[i] + (1-rotate[i])*h[i] <= Y);
        }
        // Add constraints: no overlap between each pair of blocks
        for (int i = 0; i < numBlocks - 1; i++) {
            for (int j = i + 1; j < numBlocks; j++) {
                int jj = j - (i + 1);
                model.addConstr(x[i] + rotate[i]*h[i] + (1-rotate[i])*w[i] <= x[j] + chipwidth*(xx[i][jj] + yy[i][jj]));
                model.addConstr(x[i] - rotate[j]*h[j] - (1-rotate[j])*w[j] >= x[j] - chipwidth*(1 + xx[i][jj] - yy[i][jj]));
                model.addConstr(y[i] + rotate[i]*w[i] + (1-rotate[i])*h[i] <= y[j] + MAX_H*(1 - xx[i][jj] + yy[i][jj]));
                model.addConstr(y[i] - rotate[j]*w[j] - (1-rotate[j])*h[j] >= y[j] - MAX_H*(2 - xx[i][jj] - yy[i][jj]));
            }
        }
        // Optimize model
        model.optimize();

        // =============== Results Output ===============
        cout << "\n=============== Results Output ===============\nOptimum Height: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

        log_verbose << chipwidth << " " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        for (int i = 0; i < numBlocks; i++) {
            cout << i << " : " << x[i].get(GRB_DoubleAttr_X) << " " << y[i].get(GRB_DoubleAttr_X) << " " << rotate[i].get(GRB_DoubleAttr_X) << endl;
            log_verbose << i << " : " << x[i].get(GRB_DoubleAttr_X) << " " << y[i].get(GRB_DoubleAttr_X) << " " << w[i] << " " << h[i] << " " << rotate[i].get(GRB_DoubleAttr_X) << endl;
        }
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
}