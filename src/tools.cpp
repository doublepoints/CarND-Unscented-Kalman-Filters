#include <iostream>
#include "tools.h"
#include <list>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
  VectorXd rmse(4);
  rmse << 0,0,0,0;

  // check the validity of the following inputs:
  //  * the estimation vector size should not be zero
  //  * the estimation vector size should equal ground truth vector size
  if(estimations.size() != ground_truth.size()
  		|| estimations.size() == 0){
    	cout << "Invalid estimation or ground_truth data" << endl;
		return rmse;
  }

  //accumulate squared residuals
  for(unsigned int i=0; i < estimations.size(); ++i){

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
  }

  //calculate the mean
  rmse = rmse/estimations.size();

  //calculate the squared root
  rmse = rmse.array().sqrt();

  //return the result
  return rmse;
}

void Tools::PrintGraph(const std::list<double> &data, const std::string fileName,
		       const double reference, const std::string title,
		       const std::string xTitle, const std::string yTitle){
  
  //open a pipe to gnuplot
  static FILE *gnuplotPipe = NULL;
  if (gnuplotPipe==NULL){
    gnuplotPipe= popen("gnuplot -persist", "w");  
  }  

  //print graph
  if(gnuplotPipe){
    //prepare graph
    fprintf(gnuplotPipe, "reset\n"); //gnuplot commands    
    fprintf(gnuplotPipe, "set term png #output terminal and file\n");
    fprintf(gnuplotPipe, "set output '%s'\n", fileName.c_str());
    //fprintf(gnuplotPipe, "set xrange [min:max]\n");
    //fprintf(gnuplotPipe, "set yrange [0:]\n");
    fprintf(gnuplotPipe, "set style fill solid 0.5\n");    
    fprintf(gnuplotPipe, "set xlabel '%s'\n", xTitle.c_str());
    fprintf(gnuplotPipe, "set ylabel '%s'\n", yTitle.c_str());
    fprintf(gnuplotPipe, "set title '%s'\n", title.c_str());
    //fprintf(gnuplotPipe, "plot(x, sin(x))\n");
    fprintf(gnuplotPipe, "plot '-' using 2:1 title 'NIS value' with linespoint, %f  title '95% Reference'\n", reference);
    unsigned int count = 0;
    for(double nis: data){
      //ignore first point 
      if(count < 1) {count++; continue;}
      fprintf(gnuplotPipe, "%f ", nis);
      fprintf(gnuplotPipe, "%d ", count);
      fprintf(gnuplotPipe, "\n");
      count++;
    }
    
    //flush pipe
    fflush(gnuplotPipe); 

    //close pipe
    fprintf(gnuplotPipe,"exit \n");   
    pclose(gnuplotPipe);    
    gnuplotPipe = NULL;
    std::cout << "Grap Plot finished\n";
  }
}