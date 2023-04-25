#ifndef SOLVE_H 
#define SOLVE_H

#include <vector>

#include "Steps&Grids.h"
#include "PackedMatrix.h"
#include "Vector.h"

PackedMatrix fillMatrixA(int x_n, int y_n, std::vector<double>xMain,
	///std::vector<double>xMinusHalf, std::vector<double> xPlusHalf,
	std::vector<double>xAux,
	std::vector<double> yMain,
	///std::vector<double> yPlusHalf,std::vector<double> yMinusHalf,
	std::vector<double>yAux,
	double hx, double hy, double (*k1)(double x, double y),
	double (*k2)(double x, double y), double (*g1)(double y),
	double (*g2)(double y), double (*g3)(double x),
	double (*g4)(double x), double lambda_1,
	double lambda_3,
	double (*f)(double x, double y)) {
	int N = x_n * y_n;
	int M = x_n;
	Vector g(N);
	PackedMatrix A= PackedMatrix();
	double a_m, c_m, b_m,gValue = 0;
	for (int j = 0; j < y_n; j++) {
		for (int i = 0; i < x_n; i++) {
			int m = j * M + i;////��������� � ������������� ������

			if ((i == 0) && (j == 0)) {////����� � ������������ 0,0
				c_m=hx/2*lambda_3
					+hx*(2*hy)*k2(xMain[i],yAux[j+1])
					+hy/2*lambda_1
					+hy/(2*hx)*k1(xAux[i+1],yMain[i]);
				gValue = hx / 2 * hy / 2 * f(xMain[i], yMain[j]
					+hx / 2 * g3(xMain[i])
					+ hy / 2 * g1(yMain[j]));
				g.setElem(gValue, m);
				A.putElement(c_m, m, true);
			}

			if ((j == 0) && (i > 0) && (i <= x_n - 1)) {////������ �������
				b_m = -hy / (2 * hx) * k1(xAux[i], yMain[j]);
				c_m = hx / hy * k2(xMain[i], yAux[j + 1])
					+ hx * lambda_3
					+ hy / (2 * hx) * k1(xAux[i], yMain[j])
					+ hy / (2 * hx) * k1(xAux[i + 1], yMain[j]);

				if (i == x_n - 1) {////����� x_n-1;0
					gValue = (hx * hy) / 2 * f(xMain[i], yMain[j]) + hx *
						g3(xMain[i])
						+ hy / (2 * hx) * k1(xAux[i + 1], yMain[j]) * g2(yMain[j]);
				}
				else {
					gValue=(hx * hy) / 2 * f(xMain[i], yMain[j]) + hx *
						g3(xMain[i]);
				}
				g.setElem(gValue, m);
				A.putElement(b_m, m - 1, true);
				A.putElement(c_m, m, false);
			}

			if (i == 0 && j > 0 && j <= y_n - 1) {////����� �������
				c_m = hx / (2 * hy) * k2(xMain[i], yAux[j])
					+ hx / (2 * hy) * k2(xMain[i], yAux[j + 1])
					+ hy / hx * k1(xAux[i + 1], yMain[j])
					+ hy * lambda_1;
				a_m = -hx / (2 * hy) * k2(xMain[i], yAux[j]);
				if (j == y_n - 1) {///����� 0;y_n - 1
					gValue= hx / 2 * hy * f(xMain[i], yMain[j])
						+ hy * g1(yMain[j])
						+ hx / (2 * hy) * k2(xMain[i], yAux[j + 1]) * g4(xMain[i]));
				}
				else {
					gValue= hx / 2 * hy * f(xMain[i], yMain[j]) + hy *
						g1(yMain[j]);
				}
				g.setElem(gValue, m);
				A.putElement(a_m, m - x_n, true);
				A.putElement(c_m, m, false);
			}

			if (i > 0 && i <= x_n - 1 && j > 0 && j <= y_n - 1) {////���������� �����
				c_m = hx / hy * k2(xMain[i], yAux[j])
					+ hx / hy * k2(xMain[i], yAux[j + 1])
					+ hy / hx * k1(xAux[i], yMain[j])
					+ hy / hx * k1(xAux[i + 1], yMain[j]);
				b_m = -hy / hx * k1(xAux[i], yMain[j]);
				a_m = -hx / hy * k2(xMain[i], yAux[j]);
				if ((i == x_n - 1) && (j == y_n - 1)) {
					gValue= hx * hy * f(xMain[i], yMain[j])
						+ hy / hx * k1(xAux[i + 1], yMain[j]) * g2(yMain[j])
						+ hx / hy * k2(xMain[i], yAux[j + 1]) * g4(xMain[i]));
				}
				else if (i == x_n - 1) {
					gValue= hx * hy * f(xMain[i], yMain[j]) 
						+ hy / hx * k1(xAux[i+ 1], yMain[j]) * g2(yMain[j]);
				}
				else if (j == y_n - 1) {
					gValue=hx * hy * f(xMain[i], yMain[j]) 
						+ hx / hy * k2(xMain[i],
						yAux[j + 1]) * g4(xMain[i]));
				}
				else {
					gValue = hx * hy * f(xMain[i], yMain[j]);
				}
				g.setElem(gValue, m);
				A.putElement(a_m, m - x_n, true);
				A.putElement(b_m, m - 1, false);
				A.putElement(c_m,  m, false);
			}
		}
	}

}

//
//
//std::vector<std::vector<double>> solve() {
//	double x_lower = 0;
//	double x_upper = 1;
//	int partsAmountX = 100;
//
//	double y_lower = 0;
//	double y_upper = 1;
//	int partsAmountY = 100;
//
//	std::vector<double> x_main_steps = getMainSteps(x_lower, x_upper, partsAmountX);
//	std::vector<double> x_aux_steps = getAuxiliarySteps(x_lower, x_main_steps);
//
//	std::vector<double> x_main_grid = getMainGrid(x_lower, x_main_steps);
//
//	std::vector<double> x_aux_minus_half_grid = getAuxiliaryMinusHalfGrid(x_lower, x_main_grid);
//	std::vector<double> x_aux_plus_half_grid = getAuxiliaryPlusHalfGrid(x_lower, x_main_grid);
//
//	std::vector<double> y_main_steps = getMainSteps(y_lower, y_upper, partsAmountY);
//	std::vector<double> y_aux_steps = getAuxiliarySteps(y_lower, y_main_steps);
//
//	std::vector<double> y_main_grid = getMainGrid(y_lower, y_main_steps);
//
//	std::vector<double> y_aux_minus_half_grid = getAuxiliaryMinusHalfGrid(y_lower, y_main_grid);
//	std::vector<double> y_aux_plus_half_grid = getAuxiliaryPlusHalfGrid(y_lower, y_main_grid);
//
//
//	//double h_equal = (R - zero) / n;//��� �������� �����, ����� �����������
//	//std::vector<double> mainGrid(n + 1, h_equal);///������ � ������ �������� �����
//
//	//std::vector<double> rMain(n + 1);///r �� �������� �����(����������� ������ � ������������ ������)
//	//for (int i = 0; i < n + 1; i++) {
//	//	if (i == 0) {
//	//		rMain[i] = zero;
//	//	}
//	//	else {
//	//		rMain[i] = rMain[i - 1] + mainGrid[i];
//	//	}
//	//}
//	//std::vector<double> auxiliaryGrid(n + 1);//������ � ������ �������������� �����
//	//for (int i = 0; i < n + 1; i++) {
//	//	if (i == 0) {
//	//		auxiliaryGrid[i] = mainGrid[i + 1] / 2;
//	//	}
//	//	else if (i == n) {
//	//		auxiliaryGrid[i] = mainGrid[i] / 2;
//	//	}
//	//	else {
//	//		auxiliaryGrid[i] = (mainGrid[i] + mainGrid[i + 1]) / 2;
//	//	}
//	//}
//	//std::vector<double> rAuxMinusHalf(n + 1);///r -1/2
//	//for (int i = 1; i < n + 1; i++) {
//	//	rAuxMinusHalf[i] = (rMain[i] + rMain[i - 1]) / 2;
//	//}
//	//std::vector<double> rAuxPlusHalf(n + 1);///r +1/2
//	//for (int i = 0; i < n; i++) {
//	//	rAuxPlusHalf[i] = (rMain[i] + rMain[i + 1]) / 2;
//	//}
//	std::vector < std::vector <double>> A(n + 1, std::vector<double>(n + 1));///����������� ������� �� ����� �����
//	std::vector<double> g(n + 1);
//	std::vector<std::vector<double>> v(n + 1, std::vector<double>(amountOfSteps + 1));///������, ���������� �������
//
//	for (int i = 0; i < n + 1; i++) {////�������������� �������������� �������
//		v[i][0] = u0[i];
//	}
//
//	for (double step = 1; step < amountOfSteps + 1; step++) {///����������� �� ���� ���������� �� �������
//		double t = (step - 1) * tau;
//		for (int i = 0; i < n + 1; i++) {///��������� �������� �������
//			if (i == 0) {
//				A[i][i] = -(rAuxPlusHalf[i] * k(rAuxPlusHalf[i], t)) / mainGrid[i + 1] - auxiliaryGrid[i] * rAuxPlusHalf[i] * q(rMain[i], t) / 2;
//				A[i][i + 1] = (rAuxPlusHalf[i] * k(rAuxPlusHalf[i], t)) / mainGrid[i];
//				g[i] = auxiliaryGrid[i] * rAuxPlusHalf[i] * f(rMain[i], t) / 2;
//			}
//			else if (i == n) {
//				A[i][i] = -(rAuxMinusHalf[i] * k(rAuxMinusHalf[i], t)) / mainGrid[i] - auxiliaryGrid[i] * rMain[i] * q(rMain[i], t);
//				A[i][i - 1] = (rAuxMinusHalf[i] * k(rAuxMinusHalf[i], t)) / mainGrid[i];
//				g[i] = auxiliaryGrid[i] * rMain[i] * f(rMain[i], t) + rMain[i] * nu(rMain[i], t);
//			}
//			else {
//				A[i][i] = -(rAuxMinusHalf[i] * k(rAuxMinusHalf[i], t)) / mainGrid[i] - (rAuxPlusHalf[i] * k(rAuxPlusHalf[i], t)) / mainGrid[i] - auxiliaryGrid[i] * rMain[i] * q(rMain[i], t);
//				A[i][i - 1] = (rAuxMinusHalf[i] * k(rAuxMinusHalf[i], t)) / mainGrid[i];
//				A[i][i + 1] = (rAuxPlusHalf[i] * k(rAuxPlusHalf[i], t)) / mainGrid[i + 1];
//				g[i] = auxiliaryGrid[i] * rMain[i] * f(rMain[i], t);
//			}
//		}
//
//		std::vector<std::vector<double>> inverceD(n + 1, std::vector<double>(n + 1));
//
//		for (int i = 0; i < n + 1; i++) {///�������������� �������� D �������
//			if (i == 0) {
//				inverceD[i][i] = 2 / (rAuxPlusHalf[i] * auxiliaryGrid[i]);
//			}
//			else {
//				inverceD[i][i] = 1 / (rMain[i] * auxiliaryGrid[i]);
//			}
//		}
//
//		///������� B � C
//		std::vector<std::vector<double>> B = matrixMultiplication(inverceD, A);
//		std::vector<double> C = matrixAndColumnMultiplication(inverceD, g);
//
//
//		if (isExpicit) {///���� ���������� ����� ������� ����� ������� ������
//			std::vector<double> vMinusOneColumn(n + 1);///�������������� i-1 �� ������� v ��� ������
//			std::vector<double> v_i;
//			for (int j = 0; j < A.size(); j++) {
//				vMinusOneColumn[j] = v[j][step - 1];
//			}
//			v_i = explicitEulerMethod(B, C, tau, vMinusOneColumn);
//
//			for (int j = 0; j < A.size(); j++) {////�������������� i ��� ������� �������������� �������
//				v[j][step] = v_i[j];
//			}
//		}
//		else {///���� ���������� ����� ������� ������� ������� ������
//			std::vector<double> vMinusOneColumn(n + 1);///�������������� i-1 �� ������� v ��� ������
//			std::vector<double> v_i;
//			for (int j = 0; j < A.size(); j++) {
//				vMinusOneColumn[j] = v[j][step - 1];
//			}
//			v_i = implicitEulerMethod(B, C, tau, vMinusOneColumn);
//
//			for (int j = 0; j < A.size(); j++) {////�������������� i ��� ������� �������������� �������
//				v[j][step] = v_i[j];
//			}
//		}
//	}
//	return v;
//}
#endif
