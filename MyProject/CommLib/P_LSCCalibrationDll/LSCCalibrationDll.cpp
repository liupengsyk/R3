// Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
// Qualcomm Technologies Proprietary and Confidential.

// LSCCalibrationDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>

//#define //CAM_MSG(x) (do {} while(0);)

#ifndef max
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif

#define ROLLOFF_MESH_FRACTIONAL_PART   12

#define MESH_X_REGIONS 17
#define MESH_Y_REGIONS 13

typedef struct LensRolloffMeshStruct
{
	int width;
	int height;
	int Nx;
	int Ny;
	int x_region_size;
	int y_region_size; 
	int r_gain[MESH_X_REGIONS * MESH_Y_REGIONS];
	int b_gain[MESH_X_REGIONS * MESH_Y_REGIONS];
	int gr_gain[MESH_X_REGIONS * MESH_Y_REGIONS];
	int gb_gain[MESH_X_REGIONS * MESH_Y_REGIONS];
	int percentR, percentGR, percentGB, percentB, percent;
	float r_gainf[MESH_X_REGIONS * MESH_Y_REGIONS];
	float b_gainf[MESH_X_REGIONS * MESH_Y_REGIONS];
	float gr_gainf[MESH_X_REGIONS * MESH_Y_REGIONS];
	float gb_gainf[MESH_X_REGIONS * MESH_Y_REGIONS];
	int use_float;
	int interpolation_method;
} LensRolloffMeshStruct;

int find_max_radius2(int center_x, int center_y, int width, int height)
{
	int r2, r2max, tmpx,tmpy;
	// from center to upper left corner
	r2max= center_x*center_x + center_y*center_y;
	// from center to upper right corner
	tmpx=width-1-center_x;
	r2=tmpx*tmpx+center_y*center_y;
	if (r2>r2max) 
		r2max=r2;
	// from center to lower left corner
	tmpy=height-1-center_y;
	r2=center_x*center_x + tmpy*tmpy;
	if (r2>r2max) 
		r2max=r2;
	// from center to lower right corner
	tmpx=width-1-center_x;
	tmpy=height-1-center_y;
	r2=tmpx*tmpx + tmpy*tmpy;
	if (r2>r2max) 
		r2max=r2;

	return r2max;
}
//----------------------------------------------------------------------------------------

float find_min_percent(float *grid, float channel_max, int Nx, int Ny)
{
	float channel_min;
	channel_min=grid[0];  // top left corner
	if (grid[Nx-1] < channel_min)
		channel_min=grid[Nx-1];
	if (grid[(Ny-1)*Nx]< channel_min)  // bottom left corner
		channel_min = grid[(Ny-1)*Nx];
	if (grid[Nx*Ny-1]< channel_min)  // bottom right corner
		channel_min = grid[Nx*Ny-1];
	
	return (channel_min/channel_max);
}
//------------------------------------------------------------------------------------------

void generate_surface(float *surface, float channel_max, int Nx, int Ny, float radius, \
					  int center_x, int center_y, int width, int height)
{
	int i,j;
	float delta_x, delta_y;
	float dx, dy, d2, h2, h;

	delta_x=(float) width/(float) Nx;
	delta_y=(float) height/(float) Ny;

	for (i=0;i<Ny;i++)
	{
		for (j=0;j<Nx;j++)
		{
			dx=center_x-((float) j* delta_x);
			dy=center_y-((float) i* delta_y);
			d2=dx*dx+dy*dy;
			h2= radius*radius - d2;
			h= sqrt (h2);
			surface[i*Nx+j]=channel_max * (h/radius);
		}
	}
}
//-----------------------------------------------------------------------------------------------

void measure_top_left_corner(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/lrms->Nx;
	block_height=sub_height/lrms->Ny;

	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);
	// top left corner
	//w1=block_width/4;
	//w2=2*block_width/4;
	//h1=block_height/4;
	//h2=2*block_height/4;
	w1=0;
	w2=edge_w;
	h1=0;
	h2=edge_h;

	n_pix=(h2-h1)*(w2-w1);

	sum_r=0;
	sum_gr=0;
	sum_gb=0;
	sum_b=0;	
	for (ii=h1;ii<h2;ii++)
	{
		ptr=ii*sub_width+w1;
		for (jj=w1; jj<w2;jj++)
		{	
			sum_r+=p_R[ptr];
			sum_gr+=p_Gr[ptr];
			sum_gb+=p_Gb[ptr];
			sum_b+=p_B[ptr];
			ptr++;
		}  // end of loop jj
	} //end of loop ii
	ptr=0;
	r_ave[ptr]=(float) sum_r/(float) n_pix;
	gr_ave[ptr]=(float) sum_gr/(float) n_pix;
	gb_ave[ptr]=(float) sum_gb/(float) n_pix;
	b_ave[ptr]=(float) sum_b/(float) n_pix;
	return;
}
//-------------------------------------------------------------------------------------

void measure_top_right_corner(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/lrms->Nx;
	block_height=sub_height/lrms->Ny;

	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);

	// top right corner
	w1=sub_width - edge_w;
	w2=sub_width -1 ;
	h1=0;
	h2=edge_h;

	n_pix=(h2-h1)*(w2-w1);

	sum_r=0;
	sum_gr=0;
	sum_gb=0;
	sum_b=0;	
	for (ii=h1;ii<h2;ii++)
	{
		ptr=ii*sub_width+w1;
		for (jj=w1; jj<w2;jj++)
		{	
			sum_r+=p_R[ptr];
			sum_gr+=p_Gr[ptr];
			sum_gb+=p_Gb[ptr];
			sum_b+=p_B[ptr];
			ptr++;
		}  // end of loop jj
	} //end of loop ii
	ptr=lrms->Nx-1;
	r_ave[ptr]=(float) sum_r/(float) n_pix;
	gr_ave[ptr]=(float) sum_gr/(float) n_pix;
	gb_ave[ptr]=(float) sum_gb/(float) n_pix;
	b_ave[ptr]=(float) sum_b/(float) n_pix;
	return;
}
//-------------------------------------------------------------------------------------

void measure_bottom_left_corner(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/lrms->Nx;
	block_height=sub_height/lrms->Ny;

	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);
	// top right corner
	w1=0;
	w2=edge_w;
	h1=sub_height - edge_h;
	h2=sub_height - 1;

	n_pix=(h2-h1)*(w2-w1);

	sum_r=0;
	sum_gr=0;
	sum_gb=0;
	sum_b=0;	
	for (ii=h1;ii<h2;ii++)
	{
		ptr=ii*sub_width+w1;
		for (jj=w1; jj<w2;jj++)
		{	
			sum_r+=p_R[ptr];
			sum_gr+=p_Gr[ptr];
			sum_gb+=p_Gb[ptr];
			sum_b+=p_B[ptr];
			ptr++;
		}  // end of loop jj
	} //end of loop ii
	ptr=(lrms->Ny-1)*lrms->Nx;
	r_ave[ptr]=(float) sum_r/(float) n_pix;
	gr_ave[ptr]=(float) sum_gr/(float) n_pix;
	gb_ave[ptr]=(float) sum_gb/(float) n_pix;
	b_ave[ptr]=(float) sum_b/(float) n_pix;
	return;
}
//-------------------------------------------------------------------------------------

void measure_bottom_right_corner(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/lrms->Nx;
	block_height=sub_height/lrms->Ny;
	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);

	// top right corner
	//w1=(lrms->Nx-1)*block_width+ 2*block_width/4;
	//w2=(lrms->Nx-1)*block_width+ 3*block_width/4;
	//h1=(lrms->Ny-1)*block_height + 2*block_height/4;
	//h2=(lrms->Ny-1)*block_height + 3*block_height/4;
	w1=sub_width - edge_w;
	w2=sub_width - 1;
	h1=sub_height - edge_h;
	h2=sub_height - 1;

	n_pix=(h2-h1)*(w2-w1);

	sum_r=0;
	sum_gr=0;
	sum_gb=0;
	sum_b=0;	
	for (ii=h1;ii<h2;ii++)
	{
		ptr=ii*sub_width+w1;
		for (jj=w1; jj<w2;jj++)
		{	
			sum_r+=p_R[ptr];
			sum_gr+=p_Gr[ptr];
			sum_gb+=p_Gb[ptr];
			sum_b+=p_B[ptr];
			ptr++;
		}  // end of loop jj
	} //end of loop ii
	ptr=lrms->Ny*lrms->Nx-1;
	r_ave[ptr]=(float) sum_r/(float) n_pix;
	gr_ave[ptr]=(float) sum_gr/(float) n_pix;
	gb_ave[ptr]=(float) sum_gb/(float) n_pix;
	b_ave[ptr]=(float) sum_b/(float) n_pix;
	return;
}
//-------------------------------------------------------------------------------------

void measure_top_row(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int i,j,ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/(lrms->Nx-1);
	block_height=sub_height/lrms->Ny;
	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);

	i=0;
	for (j=1;j<lrms->Nx-1;j++)
	{
		// top row
		//w1=j*block_width- block_width/8;
		//w2=j*block_width+ block_width/8;
		//h1=block_height/4;
		//h2=2*block_height/4;
		w1=j*block_width- block_width/8;
		w2=j*block_width+ block_width/8;
		h1=0;
		h2=edge_h;

		n_pix=(h2-h1)*(w2-w1);

		sum_r=0;
		sum_gr=0;
		sum_gb=0;
		sum_b=0;	
		for (ii=h1;ii<h2;ii++)
		{
			ptr=ii*sub_width+w1;
			for (jj=w1; jj<w2;jj++)
			{	
				sum_r+=p_R[ptr];
				sum_gr+=p_Gr[ptr];
				sum_gb+=p_Gb[ptr];
				sum_b+=p_B[ptr];
				ptr++;
			}  // end of loop jj
		} //end of loop ii

		ptr=j;
		r_ave[ptr]=(float) sum_r/(float) n_pix;
		gr_ave[ptr]=(float) sum_gr/(float) n_pix;
		gb_ave[ptr]=(float) sum_gb/(float) n_pix;
		b_ave[ptr]=(float) sum_b/(float) n_pix;
	}
	return;

}
//-----------------------------------------------------------------------------------------

void measure_bottom_row(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int i,j,ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/(lrms->Nx-1);
	block_height=sub_height/lrms->Ny;
	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);

	i=lrms->Ny-1;
	for (j=1;j<lrms->Nx-1;j++)
	{
		
		w1=j*block_width- block_width/8;
		w2=j*block_width+ block_width/8;
		h1=sub_height - edge_h;
		h2=sub_height - 1;

		n_pix=(h2-h1)*(w2-w1);

		sum_r=0;
		sum_gr=0;
		sum_gb=0;
		sum_b=0;	
		for (ii=h1;ii<h2;ii++)
		{
			ptr=ii*sub_width+w1;
			for (jj=w1; jj<w2;jj++)
			{	
				sum_r+=p_R[ptr];
				sum_gr+=p_Gr[ptr];
				sum_gb+=p_Gb[ptr];
				sum_b+=p_B[ptr];
				ptr++;
			}  // end of loop jj
		} //end of loop ii

		ptr=j+i*lrms->Nx;
		r_ave[ptr]=(float) sum_r/(float) n_pix;
		gr_ave[ptr]=(float) sum_gr/(float) n_pix;
		gb_ave[ptr]=(float) sum_gb/(float) n_pix;
		b_ave[ptr]=(float) sum_b/(float) n_pix;
	}
	return;
}
//-----------------------------------------------------------------------------------------

void measure_left_column(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int i,j,ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/lrms->Nx;
	block_height=sub_height/(lrms->Ny-1);
	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);

	j=0;
	for (i=1;i<lrms->Ny-1;i++)
	{
		// left column
		//w1=block_width/4;
		//w2=2*block_width/4;
		//h1=i*block_height - block_height/8;
		//h2=i*block_height + block_height/8;
		w1=0;
		w2=edge_w;
		h1=i*block_height - block_height/8;
		h2=i*block_height + block_height/8;

		n_pix=(h2-h1)*(w2-w1);

		sum_r=0;
		sum_gr=0;
		sum_gb=0;
		sum_b=0;	
		for (ii=h1;ii<h2;ii++)
		{
			ptr=ii*sub_width+w1;
			for (jj=w1; jj<w2;jj++)
			{	
				sum_r+=p_R[ptr];
				sum_gr+=p_Gr[ptr];
				sum_gb+=p_Gb[ptr];
				sum_b+=p_B[ptr];
				ptr++;
			}  // end of loop jj
		} //end of loop ii

		ptr=j+i*lrms->Nx;
		r_ave[ptr]=(float) sum_r/(float) n_pix;
		gr_ave[ptr]=(float) sum_gr/(float) n_pix;
		gb_ave[ptr]=(float) sum_gb/(float) n_pix;
		b_ave[ptr]=(float) sum_b/(float) n_pix;
	}
	return;
}
//--------------------------------------------------------------------------------------------


void measure_right_column(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int i,j,ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int edge_w, edge_h;

	block_width=sub_width/lrms->Nx;
	block_height=sub_height/(lrms->Ny-1);
	edge_w=max(10, block_width/4);
	edge_h=max(10, block_height/4);

	j=lrms->Nx-1;
	for (i=1;i<lrms->Ny-1;i++)
	{
		// right column
		//w1=j*block_width+ 2*block_width/4;
		//w2=j*block_width+ 3*block_width/4;
		//h1=i*block_height - block_height/8;
		//h2=i*block_height + block_height/8;
		w1=sub_width - edge_w;
		w2=sub_width - 1;
		h1=i*block_height - block_height/8;
		h2=i*block_height + block_height/8;

		n_pix=(h2-h1)*(w2-w1);

		sum_r=0;
		sum_gr=0;
		sum_gb=0;
		sum_b=0;	
		for (ii=h1;ii<h2;ii++)
		{
			ptr=ii*sub_width+w1;
			for (jj=w1; jj<w2;jj++)
			{	
				sum_r+=p_R[ptr];
				sum_gr+=p_Gr[ptr];
				sum_gb+=p_Gb[ptr];
				sum_b+=p_B[ptr];
				ptr++;
			}  // end of loop jj
		} //end of loop ii

		ptr=j+i*lrms->Nx;
		r_ave[ptr]=(float) sum_r/(float) n_pix;
		gr_ave[ptr]=(float) sum_gr/(float) n_pix;
		gb_ave[ptr]=(float) sum_gb/(float) n_pix;
		b_ave[ptr]=(float) sum_b/(float) n_pix;
	}
	return;
}
//--------------------------------------------------------------------------------------------

void measure_nonboundary_region(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;
	float n_pix;
	int i,j,ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;

	block_width=sub_width/(lrms->Nx-1);
	block_height=sub_height/(lrms->Ny-1);

	for (i=1;i<lrms->Ny-1;i++)
	{
		for (j=1;j<lrms->Nx-1;j++)
		{
			// nonboundary grids
			w1=j*block_width - block_width/8;
			w2=j*block_width + block_width/8;
			h1=i*block_height - block_height/8;
			h2=i*block_height + block_height/8;

			n_pix=(h2-h1)*(w2-w1);

			sum_r=0;
			sum_gr=0;
			sum_gb=0;
			sum_b=0;	
			for (ii=h1;ii<h2;ii++)
			{
				ptr=ii*sub_width+w1;
				for (jj=w1; jj<w2;jj++)
				{	
					sum_r+=p_R[ptr];
					sum_gr+=p_Gr[ptr];
					sum_gb+=p_Gb[ptr];
					sum_b+=p_B[ptr];
					ptr++;
				}  // end of loop jj
			} //end of loop ii

			ptr=j+i*lrms->Nx;
			r_ave[ptr]=(float) sum_r/(float) n_pix;
			gr_ave[ptr]=(float) sum_gr/(float) n_pix;
			gb_ave[ptr]=(float) sum_gb/(float) n_pix;
			b_ave[ptr]=(float) sum_b/(float) n_pix;
		}
	}
	return;
}

void measure_OTP_data(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float *r_ave, float *gr_ave, float *gb_ave, float *b_ave)
{
	int block_width, block_height;


	block_width=sub_width/(lrms->Nx-1);
	block_height=sub_height/(lrms->Ny-1);

	int i, j , ii, jj, x_start, x_end, y_start, y_end, temp_value;

	
	int Nx =lrms->Nx;
	int Ny= lrms->Ny;

	int nx = lrms->Nx-1;
	int ny = lrms->Ny-1;

	int Bx = floor(float(sub_width)/nx);
	int By = floor(float(sub_height)/ny);


	float * MeshIn;



	for (i=0; i< ny+1; i++)
	{
		for(j=0; j< nx+1; j++)
		{

			if(i==0)
			{
				y_start = 0; 
				y_end = y_start + (By+1)/2-1;
			}
			else if(i==ny)
			{
				y_start = sub_height - (By+1)/2;
				y_end = sub_height-1;
			}
			else
			{
				y_start = floor(float(sub_height)*i/ny+0.5)-(By+1)/2;
				y_end = y_start + By-1;
			}

			if(j==0)
			{
				x_start = 0; 
				x_end = x_start + (Bx+1)/2-1;
			}
			else if(j==nx)
			{
				x_start = sub_width - (Bx+1)/2;
				x_end = sub_width-1;
			}
			else
			{
				x_start = floor(float(sub_width)*j/nx+0.5)-(Bx+1)/2;
				x_end = x_start + Bx-1;
			}



			temp_value = 0;
			for(ii=y_start; ii<=y_end; ii++)
				for(jj=x_start; jj<=x_end; jj++)
				{
					temp_value = temp_value + p_R[ii*sub_width + jj];
				}
			r_ave[i*(nx+1) + j] = (temp_value+(x_end-x_start+1)*(y_end-y_start+1)/2)/((x_end-x_start+1)*(y_end-y_start+1));

			temp_value = 0;
			for(ii=y_start; ii<=y_end; ii++)
				for(jj=x_start; jj<=x_end; jj++)
				{
					temp_value = temp_value + p_Gr[ii*sub_width + jj];
				}
			gr_ave[i*(nx+1) + j] = (temp_value+(x_end-x_start+1)*(y_end-y_start+1)/2)/((x_end-x_start+1)*(y_end-y_start+1));

			temp_value = 0;
			for(ii=y_start; ii<=y_end; ii++)
				for(jj=x_start; jj<=x_end; jj++)
				{
					temp_value = temp_value + p_Gb[ii*sub_width + jj];
				}
			gb_ave[i*(nx+1) + j] = (temp_value+(x_end-x_start+1)*(y_end-y_start+1)/2)/((x_end-x_start+1)*(y_end-y_start+1));


			temp_value = 0;
			for(ii=y_start; ii<=y_end; ii++)
				for(jj=x_start; jj<=x_end; jj++)
				{
					temp_value = temp_value + p_B[ii*sub_width + jj];
				}
			b_ave[i*(nx+1) + j] = (temp_value+(x_end-x_start+1)*(y_end-y_start+1)/2)/((x_end-x_start+1)*(y_end-y_start+1));




		}
	}


	// Adjust the phrase of boundary OTP data.

	//r channel
	MeshIn = r_ave;

	MeshIn[0*Nx+0] = MeshIn[0*Nx+0] + (MeshIn[0*Nx+0]-MeshIn[1*Nx+1])/3;
	MeshIn[0*Nx+Nx-1] = MeshIn[0*Nx+Nx-1] + (MeshIn[0*Nx+Nx-1]-MeshIn[1*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+Nx-1] = MeshIn[(Ny-1)*Nx+Nx-1] + (MeshIn[(Ny-1)*Nx+Nx-1]-MeshIn[(Ny-2)*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+0] = MeshIn[(Ny-1)*Nx+0] + (MeshIn[(Ny-1)*Nx+0]-MeshIn[(Ny-2)*Nx+1])/3;
	
	for (i=1; i<Ny-1; i++)
	{
		MeshIn[i*Nx+0] = MeshIn[i*Nx+0] + (MeshIn[i*Nx+0]-MeshIn[i*Nx+1])/3;
		MeshIn[i*Nx+Nx-1] = MeshIn[i*Nx+Nx-1] + (MeshIn[i*Nx+Nx-1]-MeshIn[i*Nx+Nx-2])/3;
	}
	for (j=1; j<Nx-1; j++)
	{
		MeshIn[0*Nx+j] = MeshIn[0*Nx+j] + (MeshIn[0*Nx+j]-MeshIn[1*Nx+j])/3;
	    MeshIn[(Ny-1)*Nx+j] = MeshIn[(Ny-1)*Nx+j] + (MeshIn[(Ny-1)*Nx+j]-MeshIn[(Ny-2)*Nx+j])/3;
	}

	//gr channel
	MeshIn = gr_ave;

	MeshIn[0*Nx+0] = MeshIn[0*Nx+0] + (MeshIn[0*Nx+0]-MeshIn[1*Nx+1])/3;
	MeshIn[0*Nx+Nx-1] = MeshIn[0*Nx+Nx-1] + (MeshIn[0*Nx+Nx-1]-MeshIn[1*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+Nx-1] = MeshIn[(Ny-1)*Nx+Nx-1] + (MeshIn[(Ny-1)*Nx+Nx-1]-MeshIn[(Ny-2)*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+0] = MeshIn[(Ny-1)*Nx+0] + (MeshIn[(Ny-1)*Nx+0]-MeshIn[(Ny-2)*Nx+1])/3;
	
	for (i=1; i<Ny-1; i++)
	{
		MeshIn[i*Nx+0] = MeshIn[i*Nx+0] + (MeshIn[i*Nx+0]-MeshIn[i*Nx+1])/3;
		MeshIn[i*Nx+Nx-1] = MeshIn[i*Nx+Nx-1] + (MeshIn[i*Nx+Nx-1]-MeshIn[i*Nx+Nx-2])/3;
	}
	for (j=1; j<Nx-1; j++)
	{
		MeshIn[0*Nx+j] = MeshIn[0*Nx+j] + (MeshIn[0*Nx+j]-MeshIn[1*Nx+j])/3;
	    MeshIn[(Ny-1)*Nx+j] = MeshIn[(Ny-1)*Nx+j] + (MeshIn[(Ny-1)*Nx+j]-MeshIn[(Ny-2)*Nx+j])/3;
	}

	//gb channel
	MeshIn = gb_ave;

	MeshIn[0*Nx+0] = MeshIn[0*Nx+0] + (MeshIn[0*Nx+0]-MeshIn[1*Nx+1])/3;
	MeshIn[0*Nx+Nx-1] = MeshIn[0*Nx+Nx-1] + (MeshIn[0*Nx+Nx-1]-MeshIn[1*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+Nx-1] = MeshIn[(Ny-1)*Nx+Nx-1] + (MeshIn[(Ny-1)*Nx+Nx-1]-MeshIn[(Ny-2)*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+0] = MeshIn[(Ny-1)*Nx+0] + (MeshIn[(Ny-1)*Nx+0]-MeshIn[(Ny-2)*Nx+1])/3;
	
	for (i=1; i<Ny-1; i++)
	{
		MeshIn[i*Nx+0] = MeshIn[i*Nx+0] + (MeshIn[i*Nx+0]-MeshIn[i*Nx+1])/3;
		MeshIn[i*Nx+Nx-1] = MeshIn[i*Nx+Nx-1] + (MeshIn[i*Nx+Nx-1]-MeshIn[i*Nx+Nx-2])/3;
	}
	for (j=1; j<Nx-1; j++)
	{
		MeshIn[0*Nx+j] = MeshIn[0*Nx+j] + (MeshIn[0*Nx+j]-MeshIn[1*Nx+j])/3;
	    MeshIn[(Ny-1)*Nx+j] = MeshIn[(Ny-1)*Nx+j] + (MeshIn[(Ny-1)*Nx+j]-MeshIn[(Ny-2)*Nx+j])/3;
	}

	//b channel
	MeshIn = b_ave;

	MeshIn[0*Nx+0] = MeshIn[0*Nx+0] + (MeshIn[0*Nx+0]-MeshIn[1*Nx+1])/3;
	MeshIn[0*Nx+Nx-1] = MeshIn[0*Nx+Nx-1] + (MeshIn[0*Nx+Nx-1]-MeshIn[1*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+Nx-1] = MeshIn[(Ny-1)*Nx+Nx-1] + (MeshIn[(Ny-1)*Nx+Nx-1]-MeshIn[(Ny-2)*Nx+Nx-2])/3;
	MeshIn[(Ny-1)*Nx+0] = MeshIn[(Ny-1)*Nx+0] + (MeshIn[(Ny-1)*Nx+0]-MeshIn[(Ny-2)*Nx+1])/3;
	
	for (i=1; i<Ny-1; i++)
	{
		MeshIn[i*Nx+0] = MeshIn[i*Nx+0] + (MeshIn[i*Nx+0]-MeshIn[i*Nx+1])/3;
		MeshIn[i*Nx+Nx-1] = MeshIn[i*Nx+Nx-1] + (MeshIn[i*Nx+Nx-1]-MeshIn[i*Nx+Nx-2])/3;
	}
	for (j=1; j<Nx-1; j++)
	{
		MeshIn[0*Nx+j] = MeshIn[0*Nx+j] + (MeshIn[0*Nx+j]-MeshIn[1*Nx+j])/3;
	    MeshIn[(Ny-1)*Nx+j] = MeshIn[(Ny-1)*Nx+j] + (MeshIn[(Ny-1)*Nx+j]-MeshIn[(Ny-2)*Nx+j])/3;
	}


	
	return;
}

void measure_awb_nonboundary_region(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
									 float* r_ave, float* gr_ave, float* gb_ave, float* b_ave)
{
	int block_width, block_height;
	float n_pix;
	int i,j,ii,jj;
	int ptr, w1,w2,h1,h2;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;

	block_width=sub_width/(lrms->Nx-1);
	block_height=sub_height/(lrms->Ny-1);
		// nonboundary grids
			w1=sub_width/2  - sub_width/32;
			w2=sub_width/2  + sub_width/32;
			h1=sub_height/2  - sub_height/32;
			h2=sub_height/2  + sub_height/32;

			n_pix=(h2-h1)*(w2-w1);

			sum_r=0;
			sum_gr=0;
			sum_gb=0;
			sum_b=0;	
			for (ii=h1;ii<h2;ii++)
			{
				ptr=ii*sub_width+w1;
				for (jj=w1; jj<w2;jj++)
				{	
					sum_r+=p_R[ptr];
					sum_gr+=p_Gr[ptr];
					sum_gb+=p_Gb[ptr];
					sum_b+=p_B[ptr];
					ptr++;
				}  // end of loop jj
			} //end of loop ii

			
			r_ave[0]  = ((float) sum_r/(float) n_pix);
			gr_ave[0] = ((float) sum_gr/(float) n_pix);
			gb_ave[0] = ((float) sum_gb/(float) n_pix);
			b_ave[0] =  ((float) sum_b/(float) n_pix);
		
	
	return;
}

void roll_off_search_center_new(short *p_image, int width, int height, int *center_x, int *center_y, float *ave)
{

	
	// fast filtering using integral image
	int  ptr, i, j, ii, jj, iii, jjj;

	float* p_f = (float*)malloc(width*height*sizeof(float));

	
	int *integral_image = (int *)malloc(height*width*sizeof(int));
	int *integral_image_ex = (int *)calloc((height+1)*(width+1), sizeof(int));

	int rs;

	int size_boxfilter = 15;


	// genererate the integral image
	rs = 0;
	for(j=0; j<width; j++) 
	{
		rs += p_image[j]; 
		integral_image[j] = rs;
	}

	for(i=1; i<height; ++i) 
	{
		rs = 0;
		for(j=0; j<width; ++j) 
		{
			rs += p_image[i*width+j]; 
			integral_image[i*width+j] = rs + integral_image[(i-1)*width+j];
		}
	}


	// Copye to extended integral iamge
	for(i=0; i<height; ++i) 
	{
		for(j=0; j<width; ++j) 
		{
			integral_image_ex[(i+1)*(width+1) + j+1] = integral_image[i*width +j];
		}
	}

	// apply the box filter
	for(i=0; i<height; ++i) 
	{
		for(j=0; j<width; ++j) 
		{
			
			// coodinate in original image
			ii =min(max(i-(size_boxfilter-1)/2, 0), height-1); 
			iii=min(max(i+(size_boxfilter-1)/2, 0), height-1); 
			jj =min(max(j-(size_boxfilter-1)/2, 0), width-1); 
			jjj=min(max(j+(size_boxfilter-1)/2, 0), width-1); 


			p_f[i*width + j]  = (integral_image_ex[(iii+1)*(width+1) +jjj+1] 
			                    + integral_image_ex[ii     *(width+1) +jj   ] 
								- integral_image_ex[ii     *(width+1) +jjj+1]  
								- integral_image_ex[(iii+1)*(width+1) +jj   ] )/double((iii-ii+1)*(jjj-jj+1));

		}
	}




	// search the max value and location

    int max_x, max_y;
	float max_value = 0; 

	int w1 = width/4; 
	int w2 = width - width/4;
	int h1 = height/4;
	int h2 = height - height/4;

	for (i=h1; i<h2; i++)
	{
		for (j=w1; j<w2; j++)
		{

			if (p_f[i*width+j] >max_value)
			{
				max_value = p_f[i*width+j];
				max_x = j;
				max_y = i;
			}
		}
	}


	// y counts from top to bottom
	*center_x=max_x;
	*center_y=max_y;
	*ave = max_value;


	// free memory
	free(p_f);
    free(integral_image);
	free(integral_image_ex);
}


// This function searches the center of a uniform image
void roll_off_search_center(short *p_image, int width, int height, int *center_x, int *center_y, float *ave)
{

	int x_start, x_end;
	int y_start, y_end;
	int max_level=0;
	int max_x, max_y;
	unsigned long max_val, block_sum;
	int i,j, ii,jj;
	int row_length;
	int row_idx;
	int ptr;
	int block_size=8;
	int cnt=0;

	// set up the search region
	x_start=width/4;
	x_end=3*width/4;
	y_start=height/4;
	y_end=3*height/4;

	// set up the initial default value
	max_x=width/2;
	max_y=height/2;
	row_length=width;
	// compute the center value
	max_val=0;
	for (ii=-block_size;ii<=block_size;ii++)
	{
		row_idx=max_y+ii;
		ptr=row_idx*row_length+(max_x-block_size);
		
		for (jj=-block_size;jj<=block_size;jj++)
		{
			max_val+=p_image[ptr++];
		}
	}
	// the center value is computed and is stored at max_val, default
	// do the search by searching for the max
	for (i=y_start; i<=y_end; i++)
	{
		for (j=x_start; j<=x_end; j++)
		{
			block_sum=0;
			for (ii=-block_size;ii<=block_size;ii++)
			{
				row_idx=(i+ii);	
				ptr=row_idx*row_length+(j-block_size);

				for (jj=-block_size;jj<=block_size;jj++)
				{
					block_sum+=p_image[ptr++];
				}
			}

			if (block_sum>max_val)
			{
				max_val=block_sum;
				max_x=j;
				max_y=i;
			}
		}
	}

	// do the section pass to find the real center
	// several region could have the same block_sum
	int left, right, top,down;
	left=9999999;
	right=-999999;
	top=9999999;
	down=-999999;
	for (i=y_start; i<=y_end; i++)
	{
		for (j=x_start; j<=x_end; j++)
		{
			block_sum=0;
			for (ii=-block_size;ii<=block_size;ii++)
			{
				row_idx=(i+ii);	
				ptr=row_idx*row_length+(j-block_size);

				for (jj=-block_size;jj<=block_size;jj++)
				{
					block_sum+=p_image[ptr++];
				}
			}

			if (block_sum==max_val)
			{
				if (j<left)
					left=j;
				if (j>right)
					right=j;

				if (i<top)
					top=i;
				if (i>down)
					down=i;
			}
		}
	}

	max_x=(left+right+1)/2;
	max_y=(top+down+1)/2;

	// y counts from top to bottom
	*center_x=max_x*2;
	*center_y=(height-1-max_y)*2;
	i=height-1-max_y;
	j=max_x;
	block_size=50;
	//report optical center 100x100 average
	block_sum=0;
	for (ii=-block_size;ii<=block_size;ii++)
	{
		row_idx=(i+ii);	
		ptr=row_idx*row_length+(j-block_size);

		for (jj=-block_size;jj<=block_size;jj++)
		{
			block_sum+=p_image[ptr++];
			cnt++;
		}
	}
	*ave=(float)block_sum / cnt;
}

//=======================================================================
// int black_offset --> 8bit black level
int bayer_to_4_channels(unsigned char *raw_image, int raw_width, int raw_height, 
					int raw_pattern, short *p_Gr, short *p_Gb, short *p_R, short *p_B)
{
  int i,j, k;
  int sub_width, sub_height;

  sub_width=raw_width/2;
  sub_height=raw_height/2;

  short *p0, *p1, *p2, *p3;

  switch (raw_pattern)
  {
  case RGGB_PATTERN:   //	0
    p0 = p_R;
    p1 = p_Gr;
    p2 = p_Gb;
    p3 = p_B;
    break;

  case GRBG_PATTERN:  //      1
    p0 = p_Gr;
    p1 = p_R;
    p2 = p_B;
    p3 = p_Gb;
    break;

  case BGGR_PATTERN:  //	2
    p0 = p_B;
    p1 = p_Gb;
    p2 = p_Gr;
    p3 = p_R;
    break;

  case GBRG_PATTERN:  //	3
    p0 = p_Gb;
    p1 = p_B;
    p2 = p_R;
    p3 = p_Gr;
    break;

  default:
	printf("error: %s() bayer pattern value is not matched! raw_pattern = %d\r\n", raw_pattern);
    return -1;
    break;
  }

  for (i=0;i<raw_height;i+=2)
  {
    for (j=0;j<raw_width;j+=2)
    {
      k = i*raw_width+j;
      *p0++ = raw_image[k];
      *p1++ = raw_image[k + 1];
      *p2++ = raw_image[k + raw_width];
      *p3++ = raw_image[k + raw_width + 1];
    }
  }

  return 0;
}



int calibrate_rolloff_mesh_4_channel_new(short *p_R,short *p_Gr,short *p_Gb, short *p_B, 
									 int sub_width, int sub_height, LensRolloffMeshStruct *lrms,
                   short Gr_black_offset, short Gb_black_offset, short R_black_offset, short B_black_offset)
				   //, CalibrationDataStruct * pCalData)
{
	int i,j,dx,dy, ii, jj, ptr, d;
	int gain_shift;
	float *r_ave = NULL, *gr_ave = NULL, *gb_ave = NULL, *b_ave = NULL,*rwb_ave = NULL, *grwb_ave = NULL, *gbwb_ave = NULL, *bwb_ave = NULL, gain;
	int *r_center_x = NULL, *r_center_y = NULL, *gr_center_x = NULL, *gr_center_y = NULL,
		*b_center_x = NULL, *b_center_y = NULL, *gb_center_x = NULL, *gb_center_y = NULL;
	float *r_oc_ave = NULL, *gr_oc_ave = NULL, *b_oc_ave = NULL, *gb_oc_ave = NULL;
	float r_max=0, gr_max=0, gb_max=0, b_max=0, g_max=0;
	unsigned int sum_r, sum_gr, sum_gb, sum_b;
	int Nx, Ny;
	int max_r_x, max_r_y, max_gr_x, max_gr_y, max_gb_x, max_gb_y, max_b_x, max_b_y;
	int w1,w2,h1,h2;
	int common_max_x, common_max_y;

	// for N+1 grids, N regions are formed
	dx=(int) ceil( (double) sub_width/(double) (lrms->Nx-1));
	dy=(int) ceil( (double) sub_height/(double) (lrms->Ny-1));

	lrms->x_region_size=dx << 1;
	lrms->y_region_size=dy << 1;

	gain_shift=1 << ROLLOFF_MESH_FRACTIONAL_PART;

#if 1
	r_ave=(float *) malloc(sizeof(float)*lrms->Nx*lrms->Ny);
	gr_ave=(float *) malloc(sizeof(float)*lrms->Nx*lrms->Ny);
	gb_ave=(float *) malloc(sizeof(float)*lrms->Nx*lrms->Ny);
	b_ave=(float *) malloc(sizeof(float)*lrms->Nx*lrms->Ny);

	rwb_ave=(float *) malloc(sizeof(float)*1);
	grwb_ave=(float *) malloc(sizeof(float)*1);
	gbwb_ave=(float *) malloc(sizeof(float)*1);
	bwb_ave=(float *) malloc(sizeof(float)*1);

	r_center_x=(int *) malloc(sizeof(int)*1);
	r_center_y=(int *) malloc(sizeof(int)*1);
	gr_center_x=(int *) malloc(sizeof(int)*1);
	gr_center_y=(int *) malloc(sizeof(int)*1);
	b_center_x=(int *) malloc(sizeof(int)*1);
	b_center_y=(int *) malloc(sizeof(int)*1);
	gb_center_x=(int *) malloc(sizeof(int)*1);
	gb_center_y=(int *) malloc(sizeof(int)*1);

	r_oc_ave=(float *) malloc(sizeof(float)*1);
	gr_oc_ave=(float *) malloc(sizeof(float)*1);
	b_oc_ave=(float *) malloc(sizeof(float)*1);
	gb_oc_ave=(float *) malloc(sizeof(float)*1);
#endif

	if (r_ave==NULL || gr_ave==NULL || gb_ave==NULL ||b_ave==NULL)
	{
		if(r_ave)  free(r_ave);
		if(gr_ave) free(gr_ave);
		if(gb_ave) free(gb_ave);
		if(b_ave)  free(b_ave);
 
		if(rwb_ave)  free(rwb_ave);
		if(grwb_ave) free(grwb_ave);
		if(gbwb_ave) free(gbwb_ave);
		if(bwb_ave)  free(bwb_ave);

		if(r_center_x)  free(r_center_x);
		if(r_center_y)  free(r_center_y);
		if(gr_center_x) free(gr_center_x);
		if(gr_center_y) free(gr_center_y);
		if(b_center_x)  free(b_center_x);
		if(b_center_y)  free(b_center_y);
		if(gb_center_x) free(gb_center_x);
		if(gb_center_y) free(gb_center_y);

		if(r_oc_ave)    free(r_oc_ave);
		if(gr_oc_ave)   free(gr_oc_ave);
		if(b_oc_ave)  free(b_oc_ave);
		if(gb_oc_ave) free(gb_oc_ave);
  
//		printf("error: %s() allocation failed!\r\n", __FUNCTION__);
		return -1;
	}
  
	//--------------------------------------------------
	// sampling at the grid, 
	//---------------------------------------------------
	//measure_top_left_corner(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
 //   measure_top_right_corner(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_bottom_left_corner(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_bottom_right_corner(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_top_row(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_bottom_row(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_left_column(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_right_column(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//measure_nonboundary_region(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	//roll_off_search_center(p_R,  sub_width, sub_height, r_center_x,  r_center_y, r_oc_ave);
	//roll_off_search_center(p_Gr, sub_width, sub_height, gr_center_x, gr_center_y, gr_oc_ave);
	//roll_off_search_center(p_B,  sub_width, sub_height, b_center_x,  b_center_y, b_oc_ave);
	//roll_off_search_center(p_Gb, sub_width, sub_height, gb_center_x, gb_center_y, gb_oc_ave);
	
	measure_OTP_data(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, r_ave, gr_ave, gb_ave,b_ave);
	measure_awb_nonboundary_region(p_R, p_Gr, p_Gb, p_B, sub_width, sub_height,lrms, rwb_ave, grwb_ave, gbwb_ave,bwb_ave);
	roll_off_search_center_new(p_R,  sub_width, sub_height, r_center_x,  r_center_y, r_oc_ave);
	roll_off_search_center_new(p_Gr, sub_width, sub_height, gr_center_x, gr_center_y, gr_oc_ave);
	roll_off_search_center_new(p_B,  sub_width, sub_height, b_center_x,  b_center_y, b_oc_ave);
	roll_off_search_center_new(p_Gb, sub_width, sub_height, gb_center_x, gb_center_y, gb_oc_ave);
		
	//black level subtraction 8 bit width
	for (i=0;i<lrms->Nx*lrms->Ny;i++)
	{
    r_ave[i] = r_ave[i]   -  (float) R_black_offset;  
    gr_ave[i] = gr_ave[i] -  (float) Gr_black_offset; 
    gb_ave[i] = gb_ave[i] -  (float) Gb_black_offset; 
    b_ave[i] = b_ave[i]   -  (float) B_black_offset;  
  }

	rwb_ave[0] = rwb_ave[0]   -  (float) R_black_offset;  
    grwb_ave[0] = grwb_ave[0] -  (float) Gr_black_offset; 
    gbwb_ave[0] = gbwb_ave[0] -  (float) Gb_black_offset; 
    bwb_ave[0] = bwb_ave[0]   -  (float) B_black_offset;  

	r_oc_ave[0]=r_oc_ave[0] - (float) R_black_offset;
	gr_oc_ave[0]=gr_oc_ave[0] - (float) Gr_black_offset;
	gb_oc_ave[0]=gb_oc_ave[0] - (float) Gb_black_offset;
	b_oc_ave[0] = b_oc_ave[0]   - (float) B_black_offset;

	//#if 1
	FILE* fp;
	FILE* fb;
    FILE* fwb;	
	unsigned short int temp;

	fp = fopen("AWB_LSC_CALIBRATION_DATA.txt","w");
	fb = fopen("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt","wb");
	fwb = fopen("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt","wb");
	fprintf(fp," %s", "LEFT LENS SHADING DATA RED Channel {");
 //   fprintf(fp," %s", "RED Channel\n {\n");

	float red_max = r_ave[0];	
	float blue_max = b_ave[0];	
	gr_max = gr_ave[0];	
	gb_max = gb_ave[0];	
	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			if ((r_ave[i*lrms->Nx+j]) > red_max)
				red_max = r_ave[i*lrms->Nx+j];
		}		
	}

	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			if ((b_ave[i*lrms->Nx+j]) > blue_max)
				blue_max = b_ave[i*lrms->Nx+j];
		}		
	}

	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			if ((gr_ave[i*lrms->Nx+j]) > gr_max)
				gr_max = gr_ave[i*lrms->Nx+j];
		}		
	}

	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			if ((gb_ave[i*lrms->Nx+j]) > gb_max)
				gb_max = gb_ave[i*lrms->Nx+j];
		}		
	}

	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			fprintf(fp," %f ", r_ave[i*lrms->Nx+j]);				
			temp = (unsigned short int)(((1023.0/red_max)*r_ave[i*lrms->Nx+j]) + 0.5);//more precision, to apply no need to convert to Q0
			//temp = r_ave[i*lrms->Nx+j];
			fwrite(&temp, sizeof(unsigned short int), 1, fb);
//			pCalData->R_LSC[i*lrms->Nx+j] = temp;
		}
		fprintf(fp,"\n");	
	}
	fprintf(fp," %s", "}");
    fprintf(fp,"%s \n", "");
    fprintf(fp," %s", "GR Channel {");
	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			fprintf(fp," %f ", gr_ave[i*lrms->Nx+j]);	
			temp = (unsigned short int)(((1023.0/gr_max)*gr_ave[i*lrms->Nx+j]) + 0.5);//more precision, to apply no need to convert to Q0
			//temp = gr_ave[i*lrms->Nx+j];
			fwrite(&temp, sizeof(unsigned short int), 1, fb);
//			pCalData->Gr_LSC[i*lrms->Nx+j] = temp;
		}
		fprintf(fp,"\n");	
	}
	fprintf(fp," %s", "}");
    fprintf(fp,"%s \n", "");
    fprintf(fp," %s", "GB Channel {");
	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			fprintf(fp," %f ", gb_ave[i*lrms->Nx+j]);	
			temp = (unsigned short int)(((1023.0/gb_max)*gb_ave[i*lrms->Nx+j]) + 0.5);//more precision, to apply no need to convert to Q0
			//temp = gb_ave[i*lrms->Nx+j];
			fwrite(&temp, sizeof(unsigned short int), 1, fb);
//			pCalData->Gb_LSC[i*lrms->Nx+j] = temp;
		}
		fprintf(fp,"\n");	
	}
	fprintf(fp," %s", "}");
    fprintf(fp,"%s \n", "");
    fprintf(fp," %s", "B Channel {");
	for (i=0;i< lrms->Ny;i++)
	{
		for(j=0;j<lrms->Nx;j++)
		{
			//fprintf(fp," %d ", (unsigned char)b_ave[i*lrms->Nx+j]);	
			fprintf(fp," %f ", b_ave[i*lrms->Nx+j]);	
			temp = (unsigned short int)(((1023.0/blue_max)*b_ave[i*lrms->Nx+j]) + 0.5);//more precision, to apply no need to convert to Q0
			//temp = b_ave[i*lrms->Nx+j];
			fwrite(&temp, sizeof(unsigned short int), 1, fb);
//			pCalData->B_LSC[i*lrms->Nx+j] = temp;
		}
		fprintf(fp,"\n");	
	}
	fprintf(fp," %s", "}");
    fprintf(fp,"%s \n", "");

	

#if 0    
	fprintf(fp,"\n", "");
	fprintf(fp," %s", "White Balance Data with center 15% area");
	fprintf(fp,"\n %s %f ", "r/gr", rwb_ave[0]/grwb_ave[0]);	
	fprintf(fp,"\n %s %f ", "b/gr", bwb_ave[0]/grwb_ave[0]);
	fprintf(fp,"\n %s %f ", "r_ave", rwb_ave[0]);	
	fprintf(fp,"\n %s %f ", "b_ave", bwb_ave[0]);
	fprintf(fp,"\n %s %f ", "gr_ave", grwb_ave[0]);	
	fprintf(fp,"\n %s %f ", "gb_ave", gbwb_ave[0]);	
	
	temp = (unsigned short int)((rwb_ave[0]/grwb_ave[0])*1024);
	fwrite(&temp, sizeof(unsigned short int), 1, fwb);
//	pCalData->AWB[k++] = temp;
	
	temp = (unsigned short int)((bwb_ave[0]/grwb_ave[0])*1024);
	fwrite(&temp, sizeof(unsigned short int), 1, fwb);
//	pCalData->AWB[k++] = temp;

	temp = (unsigned short int)((gbwb_ave[0]/grwb_ave[0])*1024);
	fwrite(&temp, sizeof(unsigned short int), 1, fwb);
//	pCalData->AWB[k++] = temp;
#endif

	fprintf(fp,"\n", "");
	fprintf(fp,"\n %s", "White Balance Data with optical Center Data");
	fprintf(fp,"\n %s %d %s %d ","r_center_x = ", r_center_x[0], "r_center_y = ", r_center_y[0]);	
	fprintf(fp,"\n %s %d %s %d ","gr_center_x = ", gr_center_x[0], "gr_center_y = ", gr_center_y[0]);		
	fprintf(fp,"\n %s %d %s %d ","b_center_x = ", b_center_x[0], "b_center_y = ", b_center_y[0]);		
	fprintf(fp,"\n %s %d %s %d ","gb_center_x = ", gb_center_x[0], "gb_center_y = ", gb_center_y[0]);	
	fprintf(fp,"\n %s %f ", "r/gr", r_oc_ave[0]/gr_oc_ave[0]);	
	fprintf(fp,"\n %s %f ", "b/gr", b_oc_ave[0]/gr_oc_ave[0]);
	fprintf(fp,"\n %s %f ", "r_oc_ave", r_oc_ave[0]);	//optical center 100x100avg
	fprintf(fp,"\n %s %f ", "b_oc_ave", b_oc_ave[0]);   //optical center 100x100avg
	fprintf(fp,"\n %s %f ", "gr_oc_ave", gr_oc_ave[0]);	//optical center 100x100avg
	fprintf(fp,"\n %s %f ", "gb_oc_ave", gb_oc_ave[0]);	//optical center 100x100avg
	
	int k = 0;
	temp = (unsigned short int)((r_oc_ave[0]/gr_oc_ave[0])*1023);//Q10
	fwrite(&temp, sizeof(unsigned short int), 1, fwb);
//	pCalData->AWB[k++] = temp;
	
	temp = (unsigned short int)((b_oc_ave[0]/gr_oc_ave[0])*1023);//Q10
	fwrite(&temp, sizeof(unsigned short int), 1, fwb);
//	pCalData->AWB[k++] = temp;
	
	temp = (unsigned short int)((gb_oc_ave[0]/gr_oc_ave[0])*1023);//Q10
	fwrite(&temp, sizeof(unsigned short int), 1, fwb);
//	pCalData->AWB[k++] = temp;

	//needed to add optical center info and optical center channel average of 100x100 pixel area to ouput.
	
	fclose(fp);
	fclose(fb);
	fclose(fwb);
//#endif 
 
	free(r_ave);
	free(gr_ave);
	free(gb_ave);
	free(b_ave);

	free(rwb_ave);
	free(grwb_ave);
	free(gbwb_ave);
	free(bwb_ave);

	free(r_center_x);
	free(r_center_y);
	free(gr_center_x);
	free(gr_center_y);
	free(b_center_x);
	free(b_center_y);
	free(gb_center_x);
	free(gb_center_y);

	free(r_oc_ave);
	free(gr_oc_ave);
	free(b_oc_ave);
	free(gb_oc_ave);

	return 0;
}


//==============================================================================================

extern "C" _declspec(dllexport) int LensCorrectionLib(unsigned char * pImage, int nWidth, int nHeight,
    int CFA_pattern, short R_black_offset, short Gr_black_offset,
    short Gb_black_offset, short B_black_offset, bool bMode9x7)//, CalibrationDataStruct *pCalData)
{
	short * p_Gr = NULL, * p_Gb = NULL, * p_R = NULL, * p_B = NULL;
    int sub_width, sub_height;
    LensRolloffMeshStruct * lrms;
	int ret;
 
lrms = (LensRolloffMeshStruct*)malloc(sizeof(LensRolloffMeshStruct));

	if (bMode9x7)
	{
		lrms->Nx=9;
		lrms->Ny=7;
	}
	else
	{
		lrms->Nx=MESH_X_REGIONS;
		lrms->Ny=MESH_Y_REGIONS;
	}

	lrms->width=nWidth;
	lrms->height=nHeight;
    lrms->percent = 10;
	lrms->percentR=10;
	lrms->percentGR=10;
	lrms->percentGB=10;
	lrms->percentB=10;
	lrms->use_float=1;
	lrms->interpolation_method=0;

    p_Gr = (short*)malloc((nWidth * nHeight / 4)*sizeof(short));
    p_Gb = (short*)malloc((nWidth * nHeight / 4)*sizeof(short));
    p_R =  (short*)malloc((nWidth * nHeight / 4)*sizeof(short));
    p_B =  (short*)malloc((nWidth * nHeight / 4)*sizeof(short));

    if (p_Gr==NULL || p_Gb==NULL || p_R==NULL || p_B==NULL)
    {
    	if(p_Gr) free(p_Gr);
		if(p_Gb) free(p_Gb);
		if(p_R)  free(p_R);
		if(p_B)  free(p_B);
    	if(lrms) free(lrms);

//		printf("error: %s() allocation failed!\r\n", __FUNCTION__);
         return -1;
    }

    sub_width=nWidth/2;
    sub_height=nHeight/2;
   
    ret = bayer_to_4_channels(pImage, nWidth, nHeight,
        CFA_pattern, p_Gr, p_Gb, p_R, p_B);
   
    if(ret == 0) {
	    ret = calibrate_rolloff_mesh_4_channel_new(p_R,p_Gr,p_Gb, p_B, sub_width, sub_height, lrms,
                                         Gr_black_offset,Gb_black_offset,R_black_offset,
                                         B_black_offset);//, pCalData);
	}

    free(p_Gr);
    free(p_Gb);
    free(p_R);
    free(p_B);
    free(lrms);
   
    return ret;
}
