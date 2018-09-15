// ofSFR.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ofSFR.h"
#include <Halcon.h>
#include <HalconCpp.h>
#include <SFR_Process.h>

using namespace std;

enum OUTFORMAT
{
	OUTFORMAT_RGGB =0,
	OUTFORMAT_GRBG,
	OUTFORMAT_GBRG,
	OUTFORMAT_BGGR
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CofSFRApp

BEGIN_MESSAGE_MAP(CofSFRApp, CWinApp)
END_MESSAGE_MAP()


// CofSFRApp 构造

CofSFRApp::CofSFRApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CofSFRApp 对象

CofSFRApp theApp;


// CofSFRApp 初始化

BOOL CofSFRApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


//////////	//////////	//////////	//////////	//////////	//////////	//////////	//////////	//////////	//////////
int GetAllTestTargetCenter(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* fov85Cn, CPoint* fov55Cn, CPoint* fov35Cn, CPoint* fovCn)
{
	using namespace Halcon;

	// Local iconic variables 
	Hobject  Image, ImageMean, ImageSub, ImageEmphasize, RegionFore, RegionLinesLT_RB, RegionLinesRT_LB;
	Hobject  RegionUnion1, RegionUnion;
	Hobject  ConnectedRegions, SelectedRegionsFore, RegionOpeningCorner;
	Hobject  SelectedRegions, RegionUnionCorner, Rectangle, RegionRect;
	Hobject  RegionOpening, Fov85, Fov55, Fov35, ImageReduced;
	Hobject  Region, FovCenter;

	// Local control variables 
	HTuple  Width, Height;
	HTuple  Number85, Number55, Number35, Area, RowCenter;
	HTuple  ColumnCenter, Row85, Column85;
	HTuple  Row55, Column55, Row35, Column35;

	gen_image1(&Image, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	get_image_size(Image, &Width, &Height);

	mean_image(Image, &ImageMean, Width / 2, Height / 2);
	sub_image(Image, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 1);

	threshold(ImageEmphasize, &RegionFore, 0, 80);
	gen_region_line(&RegionLinesLT_RB, 120, 160, Height - 121, Width - 161);
	gen_region_line(&RegionLinesRT_LB, 120, Width - 160, Height - 121, 161);
	union2(RegionLinesLT_RB, RegionLinesRT_LB, &RegionUnion1);

	union2(RegionUnion1, RegionFore, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegionsFore, "max_area", 70);
	opening_circle(SelectedRegionsFore, &RegionOpeningCorner, 3.5);

	connection(RegionOpeningCorner, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(4000).Append(150)).Append(150)), ((HTuple(999999).Append(600)).Append(600)));
	union1(SelectedRegions, &RegionUnionCorner);

	gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 90) / 200, (Height * 90) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 85) / 200, (Height * 85) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 80) / 200, (Height * 80) / 200);
	gen_region_contour_xld(Rectangle, &RegionRect, "margin");
	union2(RegionRect, RegionUnionCorner, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	opening_circle(SelectedRegions, &RegionOpening, 3.5);
	connection(RegionOpening, &ConnectedRegions);
	select_shape(ConnectedRegions, &Fov85, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(4000).Append(150)).Append(150)), ((HTuple(999999).Append(600)).Append(600)));
	count_obj(Fov85, &Number85);

	if (0 != (Number85 != 4))
		gen_empty_obj(&Fov85);

	//stop ()
	gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 70) / 200, (Height * 70) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 65) / 200, (Height * 65) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 60) / 200, (Height * 60) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 55) / 200, (Height * 55) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 50) / 200, (Height * 50) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 45) / 200, (Height * 45) / 200);
	gen_region_contour_xld(Rectangle, &RegionRect, "margin");
	union2(RegionRect, RegionUnionCorner, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	opening_circle(SelectedRegions, &RegionOpening, 3.5);
	connection(RegionOpening, &ConnectedRegions);
	select_shape(ConnectedRegions, &Fov55, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(4000).Append(150)).Append(150)), ((HTuple(999999).Append(600)).Append(600)));
	count_obj(Fov55, &Number55);
	if (0 != (Number55 != 4))
		gen_empty_obj(&Fov55);

	//stop ()
	gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 50) / 200, (Height * 50) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 45) / 200, (Height * 45) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 40) / 200, (Height * 40) / 200);
	//gen_rectangle2_contour_xld(&Rectangle, Height / 2, Width / 2, 0, (Width * 35) / 200, (Height * 35) / 200);
	gen_region_contour_xld(Rectangle, &RegionRect, "margin");
	union2(RegionRect, RegionUnionCorner, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	opening_circle(SelectedRegions, &RegionOpening, 3.5);
	connection(RegionOpening, &ConnectedRegions);
	select_shape(ConnectedRegions, &Fov35, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(4000).Append(150)).Append(150)), ((HTuple(999999).Append(600)).Append(600)));
	count_obj(Fov35, &Number35);
	//Number35 = HTuple(0);
	if (0 != (Number35 != 4))
		gen_empty_obj(&Fov35);

	//stop ()
	gen_rectangle2(&Rectangle, Height / 2, Width / 2, 0, (Width * 20) / 200, (Height * 20) / 200);
	reduce_domain(ImageEmphasize, Rectangle, &ImageReduced);
	bin_threshold(ImageReduced, &Region);
	connection(Region, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	opening_circle(SelectedRegions, &FovCenter, 3.5);

	//stop ()
	if (0 != (Number85 == 4))
		area_center(Fov85, &Area, &Row85, &Column85);

	if (0 != (Number55 == 4))
	{
		area_center(Fov55, &Area, &Row55, &Column55);
	}

	if (0 != (Number35 == 4))
		area_center(Fov35, &Area, &Row35, &Column35);

	area_center(FovCenter, &Area, &RowCenter, &ColumnCenter);

	for (int i = 0; i<4; i++)
	{
		if (0 != (Number85 == 4))
		{
			fov85Cn[i].x = Column85[i].I();
			fov85Cn[i].y = Row85[i].I();
		}

		if (0 != (Number55 == 4))
		{
			fov55Cn[i].x = Column55[i].I();
			fov55Cn[i].y = Row55[i].I();
		}

		if (0 != (Number35 == 4))
		{
			fov35Cn[i].x = Column35[i].I();
			fov35Cn[i].y = Row35[i].I();
		}

		fovCn->x = ColumnCenter[0].I();
		fovCn->y = RowCenter[0].I();
	}

	//write_image(ImageEmphasize, "bmp", 0, "E:/e.bmp");

	Image.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	RegionFore.~Hobject();
	RegionLinesLT_RB.~Hobject();
	RegionLinesRT_LB.~Hobject();
	RegionUnion1.~Hobject();
	RegionUnion.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegionsFore.~Hobject();
	RegionOpeningCorner.~Hobject();
	SelectedRegions.~Hobject();
	RegionUnionCorner.~Hobject();
	Rectangle.~Hobject();
	RegionRect.~Hobject();
	RegionOpening.~Hobject();
	Fov85.~Hobject();
	Fov55.~Hobject();
	Fov35.~Hobject();
	ImageReduced.~Hobject();
	Region.~Hobject();
	FovCenter.~Hobject();

	return 0;
}

#if 1
#if 1
#if 1
int GetAllTestTargetCenterForMotoChart(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* target)
{//M5695F00 Plus1
	using namespace Halcon;

	int ret = 0;
	// Local iconic variables 
	Hobject  Image, GrayImage, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Region, RegionOpening, Circle, RegionIntersection;
	Hobject  ConnectedRegions, RegionFillUp, SelectedRegions;
	Hobject  Regions, RegionUnionCrossLine, RegionUnionAll, RegionTrans;
	Hobject  RegionDilation, RegionIntersectionCenter;
	Hobject  RegionUnion, RegionDifferenceB, Rectangle;
	Hobject  SortedRegions, RegionDifference;

	// Local control variables 
	HTuple  Width, Height;
	HTuple  Number, Area, Row, Column, RowCenter;
	HTuple  ColumnCenter, OffsetX, tmp, Max;
	HTuple  DiffRowCenter, DiffColumnCenter;

	gen_image1(&Image, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	zoom_image_size(Image, &Image, imgWidth / 4, imgHeight / 4, "constant");
	get_image_size(Image, &Width, &Height);

	median_image(Image, &Image, "circle", 1, "mirrored");
	mean_image(Image, &ImageMean, Width / 2, Height / 2);
	sub_image(Image, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, 51, 51, 3);

	threshold(ImageEmphasize, &Region, 0, 80);
	connection(Region, &ConnectedRegions);
	area_center(ConnectedRegions, &Area, &Row, &Column);
	tuple_max(Area, &Max);
	if (0 != (Max > 1500))
	{
		select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
		difference(Region, SelectedRegions, &Region);
		opening_circle(Region, &Region, 2.5);
	}
	opening_circle(Region, &RegionOpening, 1.5);
	gen_circle(&Circle, Height / 2, Width / 2, 60);
	intersection(RegionOpening, Circle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	fill_up(ConnectedRegions, &RegionFillUp);
	select_shape_std(RegionFillUp, &SelectedRegions, "max_area", 70);
	count_obj(SelectedRegions, &Number);
	if (0 != (Number != 1))
	{
		ret = 1;
		goto FreeMemory;
	}
	area_center(SelectedRegions, &Area, &Row, &Column);

	gen_region_hline(&Regions, HTuple(HTuple(0).Rad()).Concat(HTuple(90).Rad()), Column.Concat(Row));
	union1(Regions, &RegionUnionCrossLine);

	threshold(ImageEmphasize, &Region, 0, 80);
	connection(Region, &ConnectedRegions);
	area_center(ConnectedRegions, &Area, &Row, &Column);
	tuple_max(Area, &Max);
	if (0 != (Max > 1500))
	{
		select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
		difference(Region, SelectedRegions, &Region);
		opening_circle(Region, &Region, 2.5);
	}
	opening_circle(Region, &RegionOpening, 1.5);
	fill_up(RegionOpening, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(400).Append(11)).Append(20)), ((HTuple(1200).Append(33)).Append(60)));
	union1(SelectedRegions, &RegionUnionAll);

	shape_trans(RegionUnionCrossLine, &RegionTrans, "inner_circle");
	dilation_rectangle1(RegionTrans, &RegionDilation, 60, 80);
	intersection(RegionDilation, RegionUnionAll, &RegionIntersectionCenter);
	count_obj(RegionIntersectionCenter, &Number);
	if (0 != (Number != 1))
	{
		ret = 2;
		goto FreeMemory;
	}
	area_center(RegionIntersectionCenter, &Area, &RowCenter, &ColumnCenter);

	tuple_abs((RowCenter - Height / 2.0), &DiffRowCenter);
	tuple_abs((ColumnCenter - Width / 2.0), &DiffColumnCenter);
	int diffRow = DiffRowCenter[0].I();
	int diffColumn = DiffColumnCenter[0].I();
	if ((diffRow > 40) && (diffColumn > 40))
	{
		ret = 20;
		goto FreeMemory;
	}

	OffsetX = 10;

	ColumnCenter[0] = ColumnCenter + OffsetX;
	target[0].x = ColumnCenter[0].I() * 4;
	target[0].y = RowCenter[0].I() * 4;

	union2(RegionUnionAll, RegionUnionCrossLine, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);

	difference(RegionUnionAll, SelectedRegions, &RegionDifferenceB);

	gen_rectangle1(&Rectangle, 20, 27, RowCenter - 40, ColumnCenter - 54);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		ret = 3;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column - OffsetX;
	target[1].x = tmp[0].I() * 4;
	target[1].y = Row[0].I() * 4;

	target[5].x = tmp[1].I() * 4;
	target[5].y = Row[1].I() * 4;

	target[9].x = tmp[2].I() * 4;
	target[9].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, 20, ColumnCenter + 54, RowCenter - 54, (Width - 1) - 20);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		ret = 4;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column + OffsetX;
	target[10].x = tmp[0].I() * 4;
	target[10].y = Row[0].I() * 4;

	target[6].x = tmp[1].I() * 4;
	target[6].y = Row[1].I() * 4;

	target[2].x = tmp[2].I() * 4;
	target[2].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, RowCenter + 54, 20, (Height - 1) - 20, ColumnCenter - 54);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		ret = 5;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column - OffsetX;
	target[3].x = tmp[0].I() * 4;
	target[3].y = Row[0].I() * 4;

	target[7].x = tmp[1].I() * 4;
	target[7].y = Row[1].I() * 4;

	target[11].x = tmp[2].I() * 4;
	target[11].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, RowCenter + 54, ColumnCenter + 54, (Height - 1) - 20, (Width - 1) - 20);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		ret = 6;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column + OffsetX;
	target[12].x = tmp[0].I() * 4;
	target[12].y = Row[0].I() * 4;

	target[8].x = tmp[1].I() * 4;
	target[8].y = Row[1].I() * 4;

	target[4].x = tmp[2].I() * 4;
	target[4].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, 40, ColumnCenter - 30, (RowCenter - 1) - 120, (ColumnCenter - 1) + 50);
	intersection(RegionUnionAll, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 1))
	{
		ret = 7;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column + OffsetX;
	target[13].x = tmp[0].I() * 4;
	target[13].y = Row[0].I() * 4;

	gen_rectangle1(&Rectangle, (RowCenter - 1) + 120, ColumnCenter - 30, (RowCenter - 1) + 200, (ColumnCenter - 1) + 50);
	intersection(RegionUnionAll, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 1))
	{
		ret = 8;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column + OffsetX;
	target[14].x = tmp[0].I() * 4;
	target[14].y = Row[0].I() * 4;

	gen_rectangle1(&Rectangle, (RowCenter - 1) - 40, 30, (RowCenter - 1) + 40, (ColumnCenter - 1) - 54);
	intersection(RegionUnionAll, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 2))
	{
		ret = 9;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column - OffsetX;
	target[15].x = tmp[0].I() * 4;
	target[15].y = Row[0].I() * 4;

	target[16].x = tmp[1].I() * 4;
	target[16].y = Row[1].I() * 4;

	gen_rectangle1(&Rectangle, (RowCenter - 1) - 40, (ColumnCenter - 1) + 80, (RowCenter - 1) + 40, (ColumnCenter - 1) + 280);
	intersection(RegionUnionAll, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 2))
	{
		ret = 10;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	tmp = Column + OffsetX;
	target[18].x = tmp[0].I() * 4;
	target[18].y = Row[0].I() * 4;

	target[17].x = tmp[1].I() * 4;
	target[17].y = Row[1].I() * 4;

FreeMemory:
	Image.~Hobject();
	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();
	Circle.~Hobject();
	RegionIntersection.~Hobject();
	ConnectedRegions.~Hobject();
	RegionFillUp.~Hobject();
	SelectedRegions.~Hobject();
	Regions.~Hobject();
	RegionUnionCrossLine.~Hobject();
	RegionUnionAll.~Hobject();
	RegionTrans.~Hobject();
	RegionDilation.~Hobject();
	RegionIntersectionCenter.~Hobject();
	RegionUnion.~Hobject();
	RegionDifferenceB.~Hobject();
	Rectangle.~Hobject();
	SortedRegions.~Hobject();
	RegionDifference.~Hobject();

	return ret;
}
#else
int GetAllTestTargetCenterForMotoChart(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* target)
{//M5695F00 Plus2
	using namespace Halcon;

	int ret = 0;
	// Local iconic variables 
	Hobject  GrayImage, ImageReduce, ImageSub;
	Hobject  ImageEmphasize, Region, RegionOpening, Circle, RegionIntersection;
	Hobject  ConnectedRegions, RegionFillUp, SelectedRegions;
	Hobject  Regions, RegionUnionCrossLine, RegionUnionAll, RegionTrans;
	Hobject  RegionDilation, RegionIntersectionCenter;
	Hobject  RegionUnion, RegionDifferenceB, Rectangle;
	Hobject  SortedRegions, RegionDifference;

	// Local control variables 
	HTuple  Width, Height;
	HTuple  Number, Area, Row, Column, RowCenter;
	HTuple  ColumnCenter, OffsetX, tmp;
	HTuple  DiffRowCenter, DiffColumnCenter;

	gen_image1(&GrayImage, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);
	median_image(GrayImage, &GrayImage, "circle", 1, "mirrored");

	gen_circle(&Circle, Height / 2, Width / 2, 500.5);
	reduce_domain(GrayImage, Circle, &ImageReduce);
	threshold(ImageReduce, &Region, 0, 80);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, (((HTuple("circularity").Append("area")).Append("width")).Append("height")), "and", (((HTuple(0.8).Append(500)).Append(15)).Append(15)), (((HTuple(1).Append(2000)).Append(60)).Append(60)));
	count_obj(SelectedRegions, &Number);
	if (0 != (Number != 4))
	{
		ret = 1;
		goto FreeMemory;
	}
	sort_region(SelectedRegions, &SortedRegions, "first_point", "true", "row");
	area_center(SortedRegions, &Area, &Row, &Column);

	int iRealCenterX = 0, iRealCenterY = 0;
	for (int i = 0; i< 4; i++)
	{
		iRealCenterX += Column[i].I();
		iRealCenterY += Row[i].I();
	}
	iRealCenterX /= 4;
	iRealCenterY /= 4;

	double dDistanceRef = 0.0f;
	for (int i = 0; i< 4; i++)
		dDistanceRef += sqrt(pow(Column[i].I() - iRealCenterX, 2.0) + pow(Row[i].I() - iRealCenterY, 2.0));
	dDistanceRef /= 4;

	double dRotation = atan((double)(Row[1].I() - Row[2].I()) / (double)(Column[1].I() - Column[2].I()));

	CString	strROI = _T("");

	CString	strROIName;
	CString	strROIRef;
	CString	strROIOffsetRho;
	CString	strROIOffsetTheta;

	CString	strROIPositions;
	CString	strFieldROINames;

	POINT	ptRef;
	POINT	ptROICen;

	int		iIndex = 0;

	double	dFieldRef = dDistanceRef / 0.2f;

	while (AfxExtractSubString(strROI, strFieldROIs, iIndex, _T(';')))
	{
		AfxExtractSubString(strROIName, strROI, 0, _T(','));
		AfxExtractSubString(strROIRef, strROI, 1, _T(','));
		AfxExtractSubString(strROIOffsetRho, strROI, 2, _T(','));
		AfxExtractSubString(strROIOffsetTheta, strROI, 3, _T(','));

		if (strROIName.IsEmpty() || strROIRef.IsEmpty() || strROIOffsetRho.IsEmpty() || strROIOffsetTheta.IsEmpty())
		{
			iRet = -4;
		}
		else
		{
			ptRef.x = -1;
			ptRef.y = -1;

			if (strROIRef.Left(1).CompareNoCase(_T("M")) == 0)
			{
				if (strROIRef.Mid(1, 1).CompareNoCase(_T("C")) == 0)
				{
					ptRef.x = iRealCenterX;
					ptRef.y = iRealCenterY;
				}
				else
				{
					int	iMarkIndex = _ttoi(strROIRef.Right(strROIRef.GetLength() - 1));

					if (iMarkIndex < 0 || (iMarkIndex >= 4))
					{
						iIndex++;
						continue;
					}

					ptRef.x = ptMarks[iMarkIndex].x;
					ptRef.y = ptMarks[iMarkIndex].y;
				}

				strFieldROINames.AppendFormat(_T("%s;"), strROIName);
			}
			else if (strROIRef.Left(1).CompareNoCase(_T("D")) == 0)
			{
				int	iBlockIndex = _ttoi(strROIRef.Right(strROIRef.GetLength() - 1));

				if (iBlockIndex < 0 || (iBlockIndex >(iBlockCount - 1)))
				{
					iIndex++;
					continue;
				}

				ptRef.x = ptBlocks[iBlockIndex].x;
				ptRef.y = ptBlocks[iBlockIndex].y;

				strFieldROINames.AppendFormat(_T("%s;"), strROIName);
			}
			else
			{
				iRet = -4;

				iIndex++;
				continue;
			}

			double	dRho = _ttof(strROIOffsetRho);
			double	dTheta = _ttof(strROIOffsetTheta);

			ptROICen.x = ptRef.x + int(dFieldRef * dRho * cos(dTheta * PI / 180.0 - dRotation) + 0.5);
			ptROICen.y = ptRef.y - int(dFieldRef * dRho * sin(dTheta * PI / 180.0 - dRotation) + 0.5);

			RECT	rSFRROI;

			rSFRROI.left = ptROICen.x - iROIW / 2;
			rSFRROI.right = rSFRROI.left + iROIW - 1;
			rSFRROI.top = ptROICen.y - iROIH / 2;
			rSFRROI.bottom = rSFRROI.top + iROIH - 1;

			if (rSFRROI.left < 0)					rSFRROI.left = 0;
			if (rSFRROI.right < 0)					rSFRROI.right = 0;
			if (rSFRROI.top < 0)						rSFRROI.top = 0;
			if (rSFRROI.bottom < 0)					rSFRROI.bottom = 0;

			if (rSFRROI.left >= iImgW)	rSFRROI.left = iImgW - 1;
			if (rSFRROI.right >= iImgW)	rSFRROI.right = iImgW - 1;
			if (rSFRROI.top >= iImgH)	rSFRROI.top = iImgH - 1;
			if (rSFRROI.bottom >= iImgH)	rSFRROI.bottom = iImgH - 1;

			strROIPositions.AppendFormat(_T("%d,%d,%d,%d;"), rSFRROI.left, rSFRROI.top, rSFRROI.right, rSFRROI.bottom);
		}

		iIndex++;
	}

	strFieldROINames.TrimRight(_T(";"));
	strROIPositions.TrimRight(_T(";"));

	_tcscpy_s(lpszROINames, nMaxROINamesLen, strFieldROINames);
	_tcscpy_s(lpszROIPoss, nMaxROIROIPossLen, strROIPositions);

FreeMemory:
	GrayImage.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();
	Circle.~Hobject();
	RegionIntersection.~Hobject();
	ConnectedRegions.~Hobject();
	RegionFillUp.~Hobject();
	SelectedRegions.~Hobject();
	Regions.~Hobject();
	RegionUnionCrossLine.~Hobject();
	RegionUnionAll.~Hobject();
	RegionTrans.~Hobject();
	RegionDilation.~Hobject();
	RegionIntersectionCenter.~Hobject();
	RegionUnion.~Hobject();
	RegionDifferenceB.~Hobject();
	Rectangle.~Hobject();
	SortedRegions.~Hobject();
	RegionDifference.~Hobject();

	return ret;
}
#endif
#else
int GetAllTestTargetCenterForMotoChart(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* target)
{//M5695F00
	using namespace Halcon;

	// Local iconic variables 
	Hobject  Image, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Region, RegionOpening, RegionFillUp;
	Hobject  ConnectedRegions, SelectedRegions, RegionUnionCorner;
	Hobject  Rectangle, RegionIntersectionCenter, ObjectSelected;


	// Local control variables 
	HTuple  Index, Width, Height;
	HTuple  Area, RowCenter, ColumnCenter, OffsetX, Number;
	HTuple  Row, Column, diff, Abs;

	gen_image1(&Image, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	get_image_size(Image, &Width, &Height);

	mean_image(Image, &ImageMean, Width / 64, Height / 64);
	sub_image(Image, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 10);

	threshold(ImageEmphasize, &Region, 0, 128);
	opening_circle(Region, &RegionOpening, 7.5);
	fill_up(RegionOpening, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(10000).Append(50)).Append(50)), ((HTuple(20000).Append(300)).Append(300)));
	union1(SelectedRegions, &RegionUnionCorner);

	gen_rectangle1(&Rectangle, (Height / 2) - 200, (Width / 2) - 200, (Height / 2) + 200, (Width / 2) + 200);
	intersection(Rectangle, RegionUnionCorner, &RegionIntersectionCenter);
	area_center(RegionIntersectionCenter, &Area, &RowCenter, &ColumnCenter);

	OffsetX = 40;

	ColumnCenter = ColumnCenter - OffsetX;
	target[0].x = ColumnCenter[0].I();
	target[0].y = RowCenter[0].I();

	count_obj(SelectedRegions, &Number);
	for (Index = 1; Index <= Number; Index += 1)
	{
		select_obj(SelectedRegions, &ObjectSelected, Index);
		area_center(ObjectSelected, &Area, &Row, &Column);
		if (0 != (HTuple(Row<((Height * 3) / 20)).And(Column<((Width * 3) / 20))))
		{
			Column = Column - OffsetX;
			target[1].x = Column[0].I();
			target[1].y = Row[0].I();
		}
		else if (0 != (HTuple(Row<((Height * 3) / 20)).And(Column>((Width * 17) / 20))))
		{
			Column = Column + OffsetX;
			target[2].x = Column[0].I();
			target[2].y = Row[0].I();
		}
		else if (0 != (HTuple(Row>((Height * 17) / 20)).And(Column<((Width * 3) / 20))))
		{
			Column = Column - OffsetX;
			target[3].x = Column[0].I();
			target[3].y = Row[0].I();
		}
		else if (0 != (HTuple(Row>((Height * 17) / 20)).And(Column>((Width * 17) / 20))))
		{
			Column = Column + OffsetX;
			target[4].x = Column[0].I();
			target[4].y = Row[0].I();
		}
		else if (0 != (HTuple(Row<((Height * 5) / 20)).And(Column<((Width * 5) / 20))))
		{
			Column = Column - OffsetX;
			target[5].x = Column[0].I();
			target[5].y = Row[0].I();
		}
		else if (0 != (HTuple(Row<((Height * 5) / 20)).And(Column>((Width * 15) / 20))))
		{
			Column = Column + OffsetX;
			target[6].x = Column[0].I();
			target[6].y = Row[0].I();
		}
		else if (0 != (HTuple(Row>((Height * 15) / 20)).And(Column<((Width * 5) / 20))))
		{
			Column = Column - OffsetX;
			target[7].x = Column[0].I();
			target[7].y = Row[0].I();
		}
		else if (0 != (HTuple(Row>((Height * 15) / 20)).And(Column>((Width * 15) / 20))))
		{
			Column = Column + OffsetX;
			target[8].x = Column[0].I();
			target[8].y = Row[0].I();
		}
		else if (0 != (HTuple(Row<((Height * 7) / 20)).And(Column<((Width * 7) / 20))))
		{
			Column = Column - OffsetX;
			target[9].x = Column[0].I();
			target[9].y = Row[0].I();
		}
		else if (0 != (HTuple(Row<((Height * 7) / 20)).And(Column>((Width * 13) / 20))))
		{
			Column = Column + OffsetX;
			target[10].x = Column[0].I();
			target[10].y = Row[0].I();
		}
		else if (0 != (HTuple(Row>((Height * 13) / 20)).And(Column<((Width * 7) / 20))))
		{
			Column = Column - OffsetX;
			target[11].x = Column[0].I();
			target[11].y = Row[0].I();
		}
		else if (0 != (HTuple(Row>((Height * 13) / 20)).And(Column>((Width * 13) / 20))))
		{
			Column = Column + OffsetX;
			target[12].x = Column[0].I();
			target[12].y = Row[0].I();
		}
		else if (0 != (Row<((Height * 7) / 20)))
		{
			diff = ColumnCenter - Column;
			tuple_abs(diff, &Abs);
			if (0 != (Abs<100))
			{
				Column = Column - OffsetX;
				target[13].x = Column[0].I();
				target[13].y = Row[0].I();
			}
		}
		else if (0 != (Row>((Height * 13) / 20)))
		{
			diff = ColumnCenter - Column;
			tuple_abs(diff, &Abs);
			if (0 != (Abs<100))
			{
				Column = Column - OffsetX;
				target[14].x = Column[0].I();
				target[14].y = Row[0].I();
			}
		}
		else if (0 != (Column<((Width * 4) / 20)))
		{
			diff = RowCenter - Row;
			tuple_abs(diff, &Abs);
			if (0 != (Abs<100))
			{
				Column = Column - OffsetX;
				target[15].x = Column[0].I();
				target[15].y = Row[0].I();
			}
		}
		else if (0 != (Column<((Width * 6) / 20)))
		{
			diff = RowCenter - Row;
			tuple_abs(diff, &Abs);
			if (0 != (Abs<100))
			{
				Column = Column - OffsetX;
				target[16].x = Column[0].I();
				target[16].y = Row[0].I();
			}
		}
		else if (0 != (Column>((Width * 16) / 20)))
		{
			diff = RowCenter - Row;
			tuple_abs(diff, &Abs);
			if (0 != (Abs<100))
			{
				Column = Column + OffsetX;
				target[17].x = Column[0].I();
				target[17].y = Row[0].I();
			}
		}
		else if (0 != (Column>((Width * 14) / 20)))
		{
			diff = RowCenter - Row;
			tuple_abs(diff, &Abs);
			if (0 != (Abs<100))
			{
				Column = Column + OffsetX;
				target[18].x = Column[0].I();
				target[18].y = Row[0].I();
			}
		}
	}

	Image.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();
	RegionFillUp.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionUnionCorner.~Hobject();
	Rectangle.~Hobject();
	RegionIntersectionCenter.~Hobject();
	ObjectSelected.~Hobject();

	return 0;
}
#endif
#else
int GetAllTestTargetCenterForMotoChart(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* target)
{//OGS0433
	using namespace Halcon;

	// Local iconic variables 
	Hobject  Image, GrayImage, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Region, RegionOpening, ConnectedRegions;
	Hobject  SelectedRegions, Regions, RegionUnionCrossLine;
	Hobject  RegionFillUp, RegionUnionAll, Rectangle, RegionIntersectionCenter;
	Hobject  RegionUnion, RegionDifference;
	Hobject  EmptyObject, ObjectSelected, SortedRegions, RegionDifferenceB;
	Hobject  RegionIntersection;
	Hobject  Circle, RegionTrans, RegionDilation;

	// Local control variables 
	HTuple  ImageFiles, Index, Width, Height, WindowHandle;
	HTuple  Area, Row, Column, Sqrt, Length, OffsetLine, RowCenter;
	HTuple  ColumnCenter, OffsetX;

	gen_image1(&Image, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	get_image_size(Image, &Width, &Height);

	mean_image(Image, &ImageMean, Width / 64, Height / 64);
	sub_image(Image, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 10);

	bin_threshold(ImageEmphasize, &Region);
	opening_circle(Region, &RegionOpening, 3.5);
	
	gen_circle(&Circle, Height / 2, Width / 2, 1000);
	intersection(RegionOpening, Circle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, (HTuple("circularity").Append("area")), "and", (HTuple(0.7).Append(1500)), (HTuple(1).Append(4500)));
	area_center(SelectedRegions, &Area, &Row, &Column);
	tuple_sqrt((Width*Width) + (Height*Height), &Sqrt);
	tuple_length(Area, &Length);
	if (0 != (Length<4))
		return 1;
	OffsetLine = 75;
	gen_region_hline(&Regions, HTuple(HTuple(0).Rad()).Concat(HTuple(90).Rad()), (((HTuple(Column[0]) + HTuple(Column[3])) / 2) + OffsetLine).Concat((HTuple(Row[1]) + HTuple(Row[2])) / 2));
	union1(Regions, &RegionUnionCrossLine);

	threshold(ImageEmphasize, &Region, 0, 100);
	opening_circle(Region, &RegionOpening, 7.5);
	fill_up(RegionOpening, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);
	
	select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(20000).Append(50)).Append(50)), ((HTuple(50000).Append(500)).Append(500)));
	union1(SelectedRegions, &RegionUnionAll);

	shape_trans(RegionUnionCrossLine, &RegionTrans, "inner_center");
	dilation_circle(RegionTrans, &RegionDilation, 300.5);
	intersection(RegionDilation, RegionUnionAll, &RegionIntersectionCenter);
	area_center(RegionIntersectionCenter, &Area, &RowCenter, &ColumnCenter);

	OffsetX = 60;

	HTuple Number;
	count_obj(RegionIntersectionCenter, &Number);
	ColumnCenter = ColumnCenter - OffsetX;
	if (0 != (Number == 1))
	{
		target[0].x = ColumnCenter[0].I();
		target[0].y = RowCenter[0].I();
	}

	union2(RegionUnionAll, RegionUnionCrossLine, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	dilation_circle(RegionIntersectionCenter, &RegionDilation, 107.5);
	difference(SelectedRegions, RegionDilation, &RegionDifference);
	opening_circle(RegionDifference, &RegionOpening, 3.5);
	connection(RegionOpening, &ConnectedRegions);
	area_center(ConnectedRegions, &Area, &Row, &Column);

	count_obj(ConnectedRegions, &Number);
	if (0 != (Number == 6))
	{
		Column = Column - OffsetX;
		target[13].x = Column[0].I();
		target[13].y = Row[0].I();

		target[14].x = Column[5].I();
		target[14].y = Row[5].I();
	}

	gen_empty_obj(&EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 2);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 3);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 4);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 5);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	sort_region(EmptyObject, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area, &Row, &Column);

	HTuple  tmp = Column - OffsetX;
	count_obj(SortedRegions, &Number);
	if (0 != (Number == 6))
	{
		target[15].x = tmp[0].I();
		target[15].y = Row[0].I();

		target[16].x = tmp[1].I();
		target[16].y = Row[1].I();

		tmp = Column + OffsetX;
		target[17].x = tmp[3].I();
		target[17].y = Row[3].I();

		target[18].x = tmp[2].I();
		target[18].y = Row[2].I();
	}

	difference(RegionUnionAll, SelectedRegions, &RegionDifferenceB);
	gen_rectangle1(&Rectangle, 0, 0, RowCenter, ColumnCenter);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area, &Row, &Column);

	count_obj(SortedRegions, &Number);
	if (0 != (Number == 3))
	{
		tmp = Column - OffsetX;
		target[1].x = tmp[0].I();
		target[1].y = Row[0].I();

		target[5].x = tmp[1].I();
		target[5].y = Row[1].I();

		target[9].x = tmp[2].I();
		target[9].y = Row[2].I();
	}

	gen_rectangle1(&Rectangle, 0, ColumnCenter, RowCenter, Width - 1);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area, &Row, &Column);

	count_obj(SortedRegions, &Number);
	if (0 != (Number == 3))
	{
		tmp = Column + OffsetX;
		target[10].x = tmp[0].I();
		target[10].y = Row[0].I();

		target[6].x = tmp[1].I();
		target[6].y = Row[1].I();

		target[2].x = tmp[2].I();
		target[2].y = Row[2].I();
	}

	gen_rectangle1(&Rectangle, RowCenter, 0, Height - 1, ColumnCenter);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area, &Row, &Column);

	count_obj(SortedRegions, &Number);
	if (0 != (Number == 3))
	{
		tmp = Column - OffsetX;
		target[3].x = tmp[0].I();
		target[3].y = Row[0].I();

		target[7].x = tmp[1].I();
		target[7].y = Row[1].I();

		target[11].x = tmp[2].I();
		target[11].y = Row[2].I();
	}

	gen_rectangle1(&Rectangle, RowCenter, ColumnCenter, Height - 1, Width - 1);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area, &Row, &Column);

	count_obj(SortedRegions, &Number);
	if (0 != (Number == 3))
	{
		tmp = Column + OffsetX;
		target[12].x = tmp[0].I();
		target[12].y = Row[0].I();

		target[8].x = tmp[1].I();
		target[8].y = Row[1].I();

		target[4].x = tmp[2].I();
		target[4].y = Row[2].I();
	}

	Image.~Hobject();
	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	Regions.~Hobject();
	RegionUnionCrossLine.~Hobject();
	RegionFillUp.~Hobject();
	RegionUnionAll.~Hobject();
	Rectangle.~Hobject();
	RegionIntersectionCenter.~Hobject();
	RegionUnion.~Hobject();
	RegionDifference.~Hobject();
	EmptyObject.~Hobject();
	ObjectSelected.~Hobject();
	SortedRegions.~Hobject();
	RegionDifferenceB.~Hobject();
	RegionIntersection.~Hobject();
	Circle.~Hobject();
	RegionTrans.~Hobject();
	RegionDilation.~Hobject();

	return 0;
}
#endif


int GetCenterForAF(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* fovCn)
{
	using namespace Halcon;

	// Local iconic variables 
	Hobject  Image, ImageMean, ImageSub, ImageEmphasize, Rectangle;
	Hobject  ConnectedRegions, ImageReduced;
	Hobject  Region, FovCenter, SelectedRegions;

	// Local control variables 
	HTuple  Width, Height, Area, RowCenter, ColumnCenter;

	gen_image1(&Image, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	get_image_size(Image, &Width, &Height);

	mean_image(Image, &ImageMean, Width / 2, Height / 2);
	sub_image(Image, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 1);

	gen_rectangle2(&Rectangle, Height / 2, Width / 2, 0, (Width * 20) / 200, (Height * 20) / 200);
	reduce_domain(ImageEmphasize, Rectangle, &ImageReduced);
	bin_threshold(ImageReduced, &Region);
	connection(Region, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	opening_circle(SelectedRegions, &FovCenter, 3.5);
	area_center(FovCenter, &Area, &RowCenter, &ColumnCenter);

	fovCn->x = ColumnCenter[0].I();
	fovCn->y = RowCenter[0].I();

	Image.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	Rectangle.~Hobject();
	ImageReduced.~Hobject();
	Region.~Hobject();
	FovCenter.~Hobject();

	return 0;
}


int GetCenterMotoChartForAF(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* fovCn)
{
	using namespace Halcon;

	// Local iconic variables 
	Hobject  Image, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Region, RegionOpening, RegionFillUp;
	Hobject  ConnectedRegions, SelectedRegions, RegionUnionCorner;
	Hobject  Rectangle, RegionIntersectionCenter;


	// Local control variables 
	HTuple  Width, Height;
	HTuple  Area, RowCenter, ColumnCenter, OffsetX;

	gen_image1(&Image, "byte", imgWidth, imgHeight, (long)pGrayBuff);
	get_image_size(Image, &Width, &Height);

	mean_image(Image, &ImageMean, Width / 64, Height / 64);
	sub_image(Image, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 10);

	threshold(ImageEmphasize, &Region, 0, 128);
	opening_circle(Region, &RegionOpening, 7.5);
	fill_up(RegionOpening, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);
	//select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(10000).Append(50)).Append(50)), ((HTuple(20000).Append(300)).Append(300)));
	select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(20000).Append(50)).Append(50)), ((HTuple(40000).Append(500)).Append(500)));
	union1(SelectedRegions, &RegionUnionCorner);

	gen_rectangle1(&Rectangle, (Height / 2) - 200, (Width / 2) - 200, (Height / 2) + 200, (Width / 2) + 200);
	intersection(Rectangle, RegionUnionCorner, &RegionIntersectionCenter);
	area_center(RegionIntersectionCenter, &Area, &RowCenter, &ColumnCenter);

	OffsetX = 60;

	ColumnCenter = ColumnCenter - OffsetX;
	fovCn->x = ColumnCenter[0].I();
	fovCn->y = RowCenter[0].I();

	Image.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();
	RegionFillUp.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionUnionCorner.~Hobject();
	Rectangle.~Hobject();
	RegionIntersectionCenter.~Hobject();

	return 0;
}


void AllROI_Sort(CPoint* fov85Cn, CPoint* fov55Cn, CPoint* fov35Cn, CPoint* fovCn, CPoint* outSortROI)
{
	outSortROI[0].x = fovCn->x;
	outSortROI[0].y = fovCn->y;

	for (int i = 0; i<4; i++)
	{
		if ((fov85Cn[i].x < fovCn->x) && (fov85Cn[i].y < fovCn->y))
			outSortROI[1] = fov85Cn[i];
		if ((fov85Cn[i].x > fovCn->x) && (fov85Cn[i].y < fovCn->y))
			outSortROI[2] = fov85Cn[i];
		if ((fov85Cn[i].x < fovCn->x) && (fov85Cn[i].y > fovCn->y))
			outSortROI[3] = fov85Cn[i];
		if ((fov85Cn[i].x > fovCn->x) && (fov85Cn[i].y > fovCn->y))
			outSortROI[4] = fov85Cn[i];

		if ((fov55Cn[i].x < fovCn->x) && (fov55Cn[i].y < fovCn->y))
			outSortROI[5] = fov55Cn[i];
		if ((fov55Cn[i].x > fovCn->x) && (fov55Cn[i].y < fovCn->y))
			outSortROI[6] = fov55Cn[i];
		if ((fov55Cn[i].x < fovCn->x) && (fov55Cn[i].y > fovCn->y))
			outSortROI[7] = fov55Cn[i];
		if ((fov55Cn[i].x > fovCn->x) && (fov55Cn[i].y > fovCn->y))
			outSortROI[8] = fov55Cn[i];

		if ((fov35Cn[i].x < fovCn->x) && (fov35Cn[i].y < fovCn->y))
			outSortROI[9] = fov35Cn[i];
		if ((fov35Cn[i].x > fovCn->x) && (fov35Cn[i].y < fovCn->y))
			outSortROI[10] = fov35Cn[i];
		if ((fov35Cn[i].x < fovCn->x) && (fov35Cn[i].y > fovCn->y))
			outSortROI[11] = fov35Cn[i];
		if ((fov35Cn[i].x > fovCn->x) && (fov35Cn[i].y > fovCn->y))
			outSortROI[12] = fov35Cn[i];
	}
}


int SaveRoiImageForSFR(BYTE* pRoiBuff, CRect roiRect, CString filaName)
{
	BITMAPFILEHEADER bmpFileHeader;
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmpFileHeader.bfType = 'MB';
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+roiRect.Width() * roiRect.Height() * 3;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER bmpInfoHeader;
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = roiRect.Width();
	bmpInfoHeader.biHeight = roiRect.Height();
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;

	int byteByLine = roiRect.Width() * 3 + (roiRect.Width() * 3) % 4;
	BYTE* imgBuff = new BYTE[byteByLine * roiRect.Height()];
	BYTE* tmp = imgBuff;
	for (int j = roiRect.Height() - 1; j >= 0; j--)
	{
		for (int i = 0; i<roiRect.Width(); i++)
		{
			*(tmp++) = pRoiBuff[roiRect.Width() * j + i];
			*(tmp++) = pRoiBuff[roiRect.Width() * j + i];
			*(tmp++) = pRoiBuff[roiRect.Width() * j + i];
		}
		for (int k = 0; k<(roiRect.Width() * 3) % 4; k++)
			*(tmp++) = 0x00;
	}
	
	CStdioFile file;
	if (!file.Open(_T("D:\\") + filaName, CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite))
		file.Close();
	else
	{
		file.Write(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
		file.Write(&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
		file.Write(imgBuff, roiRect.Width() * roiRect.Height() * 3);
		file.Close();
	}

	if (imgBuff != NULL)
		delete[] imgBuff;

	return 0;
}


double GetSfrValueForROI(BYTE* pYBuffData, int roiNum, CString type, int imgWidth, int imgHeight, int offsetX, int offsetY, SFR_PARA sfrPara, CPoint* sortRoi)
{
	int cx1, cy1, cx2, cy2;
	CString fn = CString(_T(""));

	BOOL dirFlgHori = FALSE;
	switch (roiNum)
	{
	case 0:
	case 2:
	case 3:
	case 6:
	case 7:
	case 10:
	case 11:
	case 14:
	case 15:
		if (type == CString(_T("TOP")))
		{
			cx1 = sortRoi[roiNum].x + offsetX - sfrPara.RoiWidth / 2;
			cy1 = sortRoi[roiNum].y - offsetY - sfrPara.RoiHeight / 2;
			cx2 = sortRoi[roiNum].x + offsetX + sfrPara.RoiWidth / 2;
			cy2 = sortRoi[roiNum].y - offsetY + sfrPara.RoiHeight / 2;
			fn.Format(_T("ROI_%.2d_TOP"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("BOTTOM")))
		{
			cx1 = sortRoi[roiNum].x - offsetX - sfrPara.RoiWidth / 2;
			cy1 = sortRoi[roiNum].y + offsetY - sfrPara.RoiHeight / 2;
			cx2 = sortRoi[roiNum].x - offsetX + sfrPara.RoiWidth / 2;
			cy2 = sortRoi[roiNum].y + offsetY + sfrPara.RoiHeight / 2;
			fn.Format(_T("ROI_%.2d_BOTTOM"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("LEFT")))
		{
			cx1 = sortRoi[roiNum].x - offsetY - sfrPara.RoiHeight / 2;
			cy1 = sortRoi[roiNum].y - offsetX - sfrPara.RoiWidth / 2;
			cx2 = sortRoi[roiNum].x - offsetY + sfrPara.RoiHeight / 2;
			cy2 = sortRoi[roiNum].y - offsetX + sfrPara.RoiWidth / 2;
			fn.Format(_T("ROI_%.2d_LEFT"), roiNum);
			dirFlgHori = FALSE;
		}
		if (type == CString(_T("RIGHT")))
		{
			cx1 = sortRoi[roiNum].x + offsetY - sfrPara.RoiHeight / 2;
			cy1 = sortRoi[roiNum].y + offsetX - sfrPara.RoiWidth / 2;
			cx2 = sortRoi[roiNum].x + offsetY + sfrPara.RoiHeight / 2;
			cy2 = sortRoi[roiNum].y + offsetX + sfrPara.RoiWidth / 2;
			fn.Format(_T("ROI_%.2d_RIGHT"), roiNum);
			dirFlgHori = FALSE;
		}
		break;
	default:
		if (type == CString(_T("TOP")))
		{
			cx1 = sortRoi[roiNum].x - offsetX - sfrPara.RoiWidth / 2;
			cy1 = sortRoi[roiNum].y - offsetY - sfrPara.RoiHeight / 2;
			cx2 = sortRoi[roiNum].x - offsetX + sfrPara.RoiWidth / 2;
			cy2 = sortRoi[roiNum].y - offsetY + sfrPara.RoiHeight / 2;
			fn.Format(_T("ROI_%.2d_TOP"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("BOTTOM")))
		{
			cx1 = sortRoi[roiNum].x + offsetX - sfrPara.RoiWidth / 2;
			cy1 = sortRoi[roiNum].y + offsetY - sfrPara.RoiHeight / 2;
			cx2 = sortRoi[roiNum].x + offsetX + sfrPara.RoiWidth / 2;
			cy2 = sortRoi[roiNum].y + offsetY + sfrPara.RoiHeight / 2;
			fn.Format(_T("ROI_%.2d_BOTTOM"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("LEFT")))
		{
			cx1 = sortRoi[roiNum].x - offsetY - sfrPara.RoiHeight / 2;
			cy1 = sortRoi[roiNum].y + offsetX - sfrPara.RoiWidth / 2;
			cx2 = sortRoi[roiNum].x - offsetY + sfrPara.RoiHeight / 2;
			cy2 = sortRoi[roiNum].y + offsetX + sfrPara.RoiWidth / 2;
			fn.Format(_T("ROI_%.2d_LEFT"), roiNum);
			dirFlgHori = FALSE;
		}
		if (type == CString(_T("RIGHT")))
		{
			cx1 = sortRoi[roiNum].x + offsetY - sfrPara.RoiHeight / 2;
			cy1 = sortRoi[roiNum].y - offsetX - sfrPara.RoiWidth / 2;
			cx2 = sortRoi[roiNum].x + offsetY + sfrPara.RoiHeight / 2;
			cy2 = sortRoi[roiNum].y - offsetX + sfrPara.RoiWidth / 2;
			fn.Format(_T("ROI_%.2d_RIGHT"), roiNum);
			dirFlgHori = FALSE;
		}
		break;
	}

	CRect rc = new CRect(cx1, cy1, cx2, cy2);
	BYTE* imgRoiBuff = new BYTE[rc.Width() * rc.Height()];
	unsigned short* imgRoiUshortBuff = new unsigned short[rc.Width() * rc.Height() * sizeof(unsigned short)];
	long posi = 0;
	for (int j = cy1; j<cy2; j++)
	{
		for (int i = cx1; i<cx2; i++)
		{
			BYTE tmp = pYBuffData[j*imgWidth + i];
			imgRoiBuff[posi] = tmp;
			imgRoiUshortBuff[posi] = (unsigned short)tmp;
			posi++;
		}
	}

	if (sfrPara.SaveRoiImage == TRUE)
		SaveRoiImageForSFR(imgRoiBuff, rc, fn + _T(".bmp"));

	double sfrFreq = 1.0f / (sfrPara.AllRoiPara[roiNum].Freq * 2.0f);
	CSFRData* sfrData = new CSFRData[256];
	double sfrVal = 0.0f;
	if (dirFlgHori == TRUE)
		sfrVal = SFRMatbmpTest(imgRoiUshortBuff, sfrFreq, sfrData, sfrPara.RoiWidth, sfrPara.RoiHeight);
	else
		sfrVal = SFRMatbmpTest(imgRoiUshortBuff, sfrFreq, sfrData, sfrPara.RoiHeight, sfrPara.RoiWidth);

	if (imgRoiBuff != NULL)
		delete[] imgRoiBuff;

	if (imgRoiUshortBuff != NULL)
		delete[] imgRoiUshortBuff;

	if (sfrData != NULL)
		delete[] sfrData;

	return sfrVal;
}


double GetSfrValueForRawBalanceROI(USHORT* pRawData, int roiNum, CString type, int imgWidth, int imgHeight, int offsetX, int offsetY, SFR_PARA sfrPara, CPoint* sortRoi)
{
	int cx1 = 0, cy1 = 0, cx2 = 0, cy2 = 0;
	CString fn = CString(_T(""));

	int roiW = 0, roiH = 0;
	BOOL dirFlgHori = FALSE;
	switch (roiNum)
	{
	case 0:
	case 2:
	case 3:
	case 6:
	case 7:
	case 10:
	case 11:
	case 14:
	case 15:
		if (type == CString(_T("TOP")))
		{
			cx1 = sortRoi[roiNum].x + offsetX - sfrPara.RoiWidth / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y - offsetY - sfrPara.RoiHeight / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_TOP"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("BOTTOM")))
		{
			cx1 = sortRoi[roiNum].x - offsetX - sfrPara.RoiWidth / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y + offsetY - sfrPara.RoiHeight / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_BOTTOM"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("LEFT")))
		{
			cx1 = sortRoi[roiNum].x - offsetY - sfrPara.RoiHeight / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y - offsetX - sfrPara.RoiWidth / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_LEFT"), roiNum);
			dirFlgHori = FALSE;
		}
		if (type == CString(_T("RIGHT")))
		{
			cx1 = sortRoi[roiNum].x + offsetY - sfrPara.RoiHeight / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y + offsetX - sfrPara.RoiWidth / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_RIGHT"), roiNum);
			dirFlgHori = FALSE;
		}
		break;
	default:
		if (type == CString(_T("TOP")))
		{
			cx1 = sortRoi[roiNum].x - offsetX - sfrPara.RoiWidth / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y - offsetY - sfrPara.RoiHeight / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_TOP"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("BOTTOM")))
		{
			cx1 = sortRoi[roiNum].x + offsetX - sfrPara.RoiWidth / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y + offsetY - sfrPara.RoiHeight / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_BOTTOM"), roiNum);
			dirFlgHori = TRUE;
		}
		if (type == CString(_T("LEFT")))
		{
			cx1 = sortRoi[roiNum].x - offsetY - sfrPara.RoiHeight / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y + offsetX - sfrPara.RoiWidth / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_LEFT"), roiNum);
			dirFlgHori = FALSE;
		}
		if (type == CString(_T("RIGHT")))
		{
			cx1 = sortRoi[roiNum].x + offsetY - sfrPara.RoiHeight / 2;
			cx1 = ((cx1 % 2) == 0) ? cx1 : cx1 + 1;
			cy1 = sortRoi[roiNum].y - offsetX - sfrPara.RoiWidth / 2;
			cy1 = ((cy1 % 2) == 0) ? cy1 : cy1 + 1;
			roiW = ((sfrPara.RoiWidth % 2) == 0) ? sfrPara.RoiWidth : sfrPara.RoiWidth + 1;
			roiH = ((sfrPara.RoiHeight % 2) == 0) ? sfrPara.RoiHeight : sfrPara.RoiHeight + 1;
			cx2 = cx1 + roiW;
			cy2 = cy1 + roiH;
			fn.Format(_T("ROI_%.2d_RIGHT"), roiNum);
			dirFlgHori = FALSE;
		}
		break;
	}

	CRect rc = new CRect(cx1, cy1, cx2, cy2);
	BYTE* imgRoiBuff = new BYTE[rc.Width() * rc.Height() * 2];
	PCutImageBufferDll((BYTE*)pRawData, imgWidth, imgHeight, 2, cx1, cy1, roiW, roiH, imgRoiBuff);

	if (sfrPara.SaveRoiImage == TRUE)
	{
		CString path = _T("D:\\") + fn + _T(".raw");
		CStdioFile file;
		file.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		file.Write(imgRoiBuff, roiW * roiH * 2);
		file.Close();
	}

	BYTE* BlockBuffer = new BYTE[roiW * roiH * 2];
	memset(BlockBuffer, 0, roiW * roiH * 2);
	int outformat = OUTFORMAT_RGGB;
	RawToGrayscaleImage_Raw10Dll(imgRoiBuff, outformat, roiW, roiH, BlockBuffer);

	if (sfrPara.SaveRoiImage == TRUE)
	{
		CString path = _T("D:\\") + fn + _T("GrayScale.raw");
		CStdioFile file;
		file.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		file.Write(BlockBuffer, roiW * roiH * 2);
		file.Close();
	}

	double sfrFreq = 1.0f / (sfrPara.AllRoiPara[roiNum].Freq * 2.0f);
	CSFRData* sfrData = new CSFRData[256];
	double sfrVal = 0.0f;
	if (dirFlgHori == TRUE)
		sfrVal = SFRMatbmpTest((USHORT*)BlockBuffer, sfrFreq, sfrData, sfrPara.RoiWidth, sfrPara.RoiHeight);
	else
		sfrVal = SFRMatbmpTest((USHORT*)BlockBuffer, sfrFreq, sfrData, sfrPara.RoiHeight, sfrPara.RoiWidth);

	if (imgRoiBuff != NULL)
		delete[] imgRoiBuff;

	if (BlockBuffer != NULL)
		delete[] BlockBuffer;

	if (sfrData != NULL)
		delete[] sfrData;

	return sfrVal;
}


double GetSfrValueForROI_MotoChart(BYTE* pYBuffData, int roiNum, int imgWidth, int imgHeight, int offsetX, int offsetY, SFR_PARA sfrPara, CPoint* sortRoi)
{
	int cx1, cy1, cx2, cy2;
	CString fn = CString(_T(""));
	
	cx1 = sortRoi[roiNum].x + offsetX - sfrPara.RoiWidth / 2;
	cy1 = sortRoi[roiNum].y - offsetY - sfrPara.RoiHeight / 2;
	cx2 = sortRoi[roiNum].x + offsetX + sfrPara.RoiWidth / 2;
	cy2 = sortRoi[roiNum].y - offsetY + sfrPara.RoiHeight / 2;
	fn.Format(_T("ROI_%.2d"), roiNum);

	CRect rc = new CRect(cx1, cy1, cx2, cy2);
	BYTE* imgRoiBuff = new BYTE[rc.Width() * rc.Height()];
	unsigned short* imgRoiUshortBuff = new unsigned short[rc.Width() * rc.Height() * sizeof(unsigned short)];
	long posi = 0;
	for (int j = cy1; j<cy2; j++)
	{
		for (int i = cx1; i<cx2; i++)
		{
			BYTE tmp = pYBuffData[j*imgWidth + i];
			imgRoiBuff[posi] = tmp;
			imgRoiUshortBuff[posi] = (unsigned short)tmp;
			posi++;
		}
	}

	if (sfrPara.SaveRoiImage == TRUE)
		SaveRoiImageForSFR(imgRoiBuff, rc, fn + _T(".bmp"));

	double sfrFreq = 1.0f / (sfrPara.AllRoiPara[roiNum].Freq * 2.0f);
	CSFRData* sfrData = new CSFRData[256];
	double sfrVal = 0.0f;
	//if (dirFlgHori == TRUE)
		sfrVal = SFRMatbmpTest(imgRoiUshortBuff, sfrFreq, sfrData, sfrPara.RoiWidth, sfrPara.RoiHeight);
	//else
		//sfrVal = SFRMatbmpTest(imgRoiUshortBuff, sfrFreq, sfrData, sfrPara.RoiHeight, sfrPara.RoiWidth);

	if (imgRoiBuff != NULL)
		delete[] imgRoiBuff;

	if (imgRoiUshortBuff != NULL)
		delete[] imgRoiUshortBuff;

	if (sfrData != NULL)
		delete[] sfrData;

	return sfrVal;
}


BOOL SFR_Test(BYTE* bmpChannelYBuff, int imgWidth, int imgHeight, SFR_PARA sfrPara, SFR_ROI_VALUE* sfrRoiValue)
{
	BYTE* TestYbuf = new BYTE[imgWidth * imgHeight];
	memset(TestYbuf, 0, imgWidth * imgHeight);

	CPoint fov85Center[4], fov55Center[4], fov35Center[4], fovCn;
	GetAllTestTargetCenter(TestYbuf, imgWidth, imgHeight, fov85Center, fov55Center, fov35Center, &fovCn);
	CPoint sortRoi[21];
	AllROI_Sort(fov85Center, fov55Center, fov35Center, &fovCn, sortRoi);

	for (int i = 0; i<21; i++)
	{
		if ((sortRoi[i].x == 0) || (sortRoi[i].y == 0))
			continue;

		int roiOffsetX = sfrPara.RoiOffsetX + sfrPara.AllRoiPara[i].OffsetX;
		int roiOffsetY = sfrPara.RoiOffsetY + sfrPara.AllRoiPara[i].OffsetY;
		if (sfrPara.AllRoiPara[i].TopRoiEn)
		{
			sfrRoiValue[i].topVal = GetSfrValueForROI(TestYbuf, i, _T("TOP"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, sortRoi);
			sfrRoiValue[i].topResult = (sfrRoiValue[i].topVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? true : false;
		}

		if (sfrPara.AllRoiPara[i].BottomRoiEn)
		{
			sfrRoiValue[i].bottomVal = GetSfrValueForROI(TestYbuf, i, _T("BOTTOM"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, sortRoi);
			sfrRoiValue[i].bottomResult = (sfrRoiValue[i].bottomVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? true : false;
		}

		if (sfrPara.AllRoiPara[i].LeftRoiEn)
		{
			sfrRoiValue[i].leftVal = GetSfrValueForROI(TestYbuf, i, _T("LEFT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, sortRoi);
			sfrRoiValue[i].leftResult = (sfrRoiValue[i].leftVal > sfrPara.AllRoiPara[i].SpecV / 1000.0f) ? true : false;
		}

		if (sfrPara.AllRoiPara[i].RightRoiEn)
		{
			sfrRoiValue[i].rightVal = GetSfrValueForROI(TestYbuf, i, _T("RIGHT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, sortRoi);
			sfrRoiValue[i].rightResult = (sfrRoiValue[i].rightVal > sfrPara.AllRoiPara[i].SpecV / 1000.0f) ? true : false;
		}
	}
	if (TestYbuf != NULL)
		delete[] TestYbuf;

	BOOL bSfrResult = TRUE;
	for (int i = 0; i<21; i++)
	{
		if ((sfrPara.AllRoiPara[i].TopRoiEn == TRUE) && (sfrRoiValue[i].topResult == FALSE))
		{
			bSfrResult = FALSE;
			break;
		}
		if ((sfrPara.AllRoiPara[i].BottomRoiEn == TRUE) && (sfrRoiValue[i].bottomResult == FALSE))
		{
			bSfrResult = FALSE;
			break;
		}
		if ((sfrPara.AllRoiPara[i].LeftRoiEn == TRUE) && (sfrRoiValue[i].leftResult == FALSE))
		{
			bSfrResult = FALSE;
			break;
		}
		if ((sfrPara.AllRoiPara[i].RightRoiEn == TRUE) && (sfrRoiValue[i].rightResult == FALSE))
		{
			bSfrResult = FALSE;
			break;
		}
	}

	return bSfrResult;
}


bool PCutImageBufferDll(unsigned char *_InImageBuffer, int _Width, int _Height, int _ByteDepth, int _mStartX, int _mStartY, int _CutWidth, int _CutHeight, unsigned char *_OutImageBuffer)
{
	bool bRes = true;

	if (((_CutWidth>_Width) || (_CutHeight>_Height)) || ((_CutWidth <= 0) || (_CutHeight <= 0)))
	{
		bRes = false;
		return bRes;
	}
	if ((_Width == _CutWidth) && (_Height == _CutHeight))
	{
		memcpy(_OutImageBuffer, _InImageBuffer, sizeof(unsigned char)*_Width*_Height*_ByteDepth);
		return bRes;
	}
	int _StartX = _mStartX;
	int _StartY = _mStartY;

	for (int i = 0; i<_ByteDepth; i++)
	{
		for (int y = 0; y<_CutHeight; y++)
		{
			for (int x = 0; x<_CutWidth; x++)
			{
				int X = _StartX + x;
				int Y = _StartY + y;
				int OutIndex = (y*_CutWidth + x)*_ByteDepth;
				int InIndex = (Y*_Width + X)*_ByteDepth;
				_OutImageBuffer[OutIndex + i] = _InImageBuffer[InIndex + i];
			}
		}
	}
	return bRes;
}


void GetBayerTypeDll(int iImageType, int iStartX, int iStartY, int& outformat)
{
	if ((iStartX % 2 == 0) && (iStartY % 2 == 1))
	{
		switch (outformat)
		{
		case 	OUTFORMAT_RGGB:
			outformat = OUTFORMAT_GBRG;
			break;
		case 	OUTFORMAT_GRBG:
			outformat = OUTFORMAT_BGGR;
			break;
		case 	OUTFORMAT_GBRG:
			outformat = OUTFORMAT_RGGB;
			break;
		case 	OUTFORMAT_BGGR:
			outformat = OUTFORMAT_GRBG;
			break;
		}
	}
	else if ((iStartX % 2 == 1) && (iStartY % 2 == 0))
	{
		switch (outformat)
		{
		case 	OUTFORMAT_RGGB:
			outformat = OUTFORMAT_GRBG;
			break;
		case 	OUTFORMAT_GRBG:
			outformat = OUTFORMAT_RGGB;
			break;
		case 	OUTFORMAT_GBRG:
			outformat = OUTFORMAT_BGGR;
			break;
		case 	OUTFORMAT_BGGR:
			outformat = OUTFORMAT_GBRG;
			break;
		}
	}
	else if ((iStartX % 2 == 1) && (iStartY % 2 == 1))
	{
		switch (outformat)
		{
		case 	OUTFORMAT_RGGB:
			outformat = OUTFORMAT_BGGR;
			break;
		case 	OUTFORMAT_GRBG:
			outformat = OUTFORMAT_GBRG;
			break;
		case 	OUTFORMAT_GBRG:
			outformat = OUTFORMAT_GRBG;
			break;
		case 	OUTFORMAT_BGGR:
			outformat = OUTFORMAT_RGGB;
			break;
		}
	}
}


void RawToGrayscaleImage_Raw10Dll(unsigned char *m_pBuff, int outformat, int m_width, int m_height, unsigned char *pOutRaw10)
{
	USHORT* m_pBuff2 = new USHORT[m_width*m_height];
	USHORT* pOut = new USHORT[m_width*m_height];
	int i = 0;
	for (i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			m_pBuff2[i*m_width + j] = (m_pBuff[(i*m_width + j) * 2 + 1] << 8) + m_pBuff[(i*m_width + j) * 2];
		}
	}

	int  PixW = m_width;
	int  PixH = m_height;

	int  StartX = 0;
	int  StartY = 0;

	int  EndX = PixW;
	int  EndY = PixH;

	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1 = 0, Pos2 = 0;

	long  SumGr = 0;
	double	Gr = 0.0;

	long  SumGb = 0;
	double Gb = 0.0;

	long  SumR = 0;
	double R = 0.0;

	long  SumB = 0;
	double B = 0.0;

	//计算Gr Avg
	if (outformat == OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos1 + 1];
				Count++;
			}
		}
	}
	if (outformat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos1];
				Count++;
			}
		}
	}

	if (outformat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos2 + 1];
				Count++;
			}
		}
	}

	if (outformat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos2];
				Count++;
			}
		}
	}
	double GrAve = Gr = SumGr / Count;
	//DebugInfo(L"ROI Avg Gr = %f",Gr);
	SumGr = 0;
	Count = 0;

	//------------------------------------------
	if (outformat == OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos1 + 1] >= Gr)
				{
					SumR += m_pBuff2[Pos1];
					SumGr += m_pBuff2[Pos1 + 1];
					SumGb += m_pBuff2[Pos2];
					SumB += m_pBuff2[Pos2 + 1];
					Count++;
				}
			}
		}
	}
	if (outformat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos1] >= Gr)
				{
					SumGr += m_pBuff2[Pos1];
					SumR += m_pBuff2[Pos1 + 1];
					SumB += m_pBuff2[Pos2];
					SumGb += m_pBuff2[Pos2 + 1];

					Count++;
				}
			}
		}
	}

	if (outformat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos2 + 1] >= Gr)
				{
					SumGb += m_pBuff2[Pos1];
					SumB += m_pBuff2[Pos1 + 1];
					SumR += m_pBuff2[Pos2];
					SumGr += m_pBuff2[Pos2 + 1];

					Count++;
				}
			}
		}
	}

	if (outformat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos2] >= Gr)
				{
					SumB += m_pBuff2[Pos1];
					SumGb += m_pBuff2[Pos1 + 1];
					SumGr += m_pBuff2[Pos2];
					SumR += m_pBuff2[Pos2 + 1];

					Count++;
				}
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R = 1.0*SumR / Count;
	B = 1.0*SumB / Count;

	//double G_Ave = (0.5*(Gr+Gb));

	double Gr_R = (Gr / R);
	double Gr_Gb = (Gr / Gb);
	double Gr_B = (Gr / B);
	//DebugInfo(L"Gr_R=%f    Gr_Gb=%f     Gr_B=%f",Gr_R,Gr_Gb,Gr_B);

	if (outformat == OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1] = ((int)(m_pBuff2[Pos1] * Gr_R)>1023 ? 1023 : (int)(m_pBuff2[Pos1] * Gr_R));
				pOut[Pos1 + 1] = m_pBuff2[Pos1 + 1];
				pOut[Pos2] = ((int)(m_pBuff2[Pos2] * Gr_Gb)>1023 ? 1023 : (int)(m_pBuff2[Pos2] * Gr_Gb));
				pOut[Pos2 + 1] = ((int)(m_pBuff2[Pos2 + 1] * Gr_B)>1023 ? 1023 : (int)(m_pBuff2[Pos2 + 1] * Gr_B));
			}
		}
	}
	if (outformat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1] = m_pBuff2[Pos1];
				pOut[Pos1 + 1] = ((int)(m_pBuff2[Pos1 + 1] * Gr_R)>1023 ? 1023 : (int)(m_pBuff2[Pos1 + 1] * Gr_R));
				pOut[Pos2] = ((int)(m_pBuff2[Pos2] * Gr_B)>1023 ? 1023 : (int)(m_pBuff2[Pos2] * Gr_B));
				pOut[Pos2 + 1] = ((int)(m_pBuff2[Pos2 + 1] * Gr_Gb)>1023 ? 1023 : (int)(m_pBuff2[Pos2 + 1] * Gr_Gb));

			}
		}
	}

	if (outformat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1] = ((int)(m_pBuff2[Pos1] * Gr_Gb)>1023 ? 1023 : (int)(m_pBuff2[Pos1] * Gr_Gb));
				pOut[Pos1 + 1] = ((int)(m_pBuff2[Pos1 + 1] * Gr_B)>1023 ? 1023 : (int)(m_pBuff2[Pos1 + 1] * Gr_B));
				pOut[Pos2] = ((int)(m_pBuff2[Pos2] * Gr_R)>1023 ? 1023 : (int)(m_pBuff2[Pos2] * Gr_R));
				pOut[Pos2 + 1] = m_pBuff2[Pos2 + 1];
			}
		}
	}

	if (outformat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1] = ((int)(m_pBuff2[Pos1] * Gr_B)>1023 ? 1023 : (int)(m_pBuff2[Pos1] * Gr_B));
				pOut[Pos1 + 1] = ((int)(m_pBuff2[Pos1 + 1] * Gr_Gb)>1023 ? 1023 : (int)(m_pBuff2[Pos1 + 1] * Gr_Gb));
				pOut[Pos2] = m_pBuff2[Pos2];
				pOut[Pos2 + 1] = ((int)(m_pBuff2[Pos2 + 1] * Gr_R)>1023 ? 1023 : (int)(m_pBuff2[Pos2 + 1] * Gr_R));
			}
		}
	}

	USHORT nCount = 0;
	for (i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			pOutRaw10[nCount++] = (pOut[i*m_width + j]) & 0xff;
			pOutRaw10[nCount++] = (pOut[i*m_width + j] >> 8) & 0x03;
		}
	}
	delete[]m_pBuff2;
	delete[]pOut;
}


int GetBestCode(CString afMode, AF_STEP_DATA stepData, int stepCode, int* startCode, int* endCode, float* bestAv)
{
	int ret = 0;

	float maxAV = 0.0f;
	int peakCode = 0, peakCnt = 0;
	if (afMode == _T("BIG_STEP_MODE"))
	{
		for (int i = 0; i<stepData.bigStepCnt; i++)
		{
			if (stepData.bigStepData[i].av > maxAV)
			{
				maxAV = stepData.bigStepData[i].av;
				peakCode = stepData.bigStepData[i].code;
				peakCnt = i;
			}
		}

		if (peakCode == stepData.bigStepData[0].code)
		{
			int tmpCode = stepData.bigStepData[0].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.bigStepData[0].code + stepCode;
			*bestAv = stepData.bigStepData[0].av;
			return stepData.bigStepData[0].code;
		}

		if (peakCode == stepData.bigStepData[stepData.bigStepCnt - 1].code)
		{
			int tmpCode = stepData.bigStepData[stepData.bigStepCnt - 1].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.bigStepData[stepData.bigStepCnt - 1].code + stepCode;
			*bestAv = stepData.bigStepData[stepData.bigStepCnt - 1].av;
			return stepData.bigStepData[stepData.bigStepCnt - 1].code;
		}
	}
	else
	{
		for (int i = 0; i<stepData.miniStepCnt; i++)
		{
			if (stepData.miniStepData[i].av > maxAV)
			{
				maxAV = stepData.miniStepData[i].av;
				peakCode = stepData.miniStepData[i].code;
				peakCnt = i;
			}
		}

		if (peakCode == stepData.miniStepData[0].code)
		{
			int tmpCode = stepData.miniStepData[0].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.miniStepData[0].code + stepCode;
			*bestAv = stepData.miniStepData[0].av;
			return stepData.miniStepData[0].code;
		}

		if (peakCode == stepData.miniStepData[stepData.miniStepCnt - 1].code)
		{
			int tmpCode = stepData.miniStepData[stepData.miniStepCnt - 1].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.miniStepData[stepData.miniStepCnt - 1].code + stepCode;
			*bestAv = stepData.miniStepData[stepData.miniStepCnt - 1].av;
			return stepData.miniStepData[stepData.miniStepCnt - 1].code;
		}
	}

	if (peakCnt < 1)
		peakCnt = 1;

	float y1 = 0.0f, y2 = 0.0f, y3 = 0.0f;
	int x1 = 0, x2 = 0, x3 = 0;
	if (afMode == _T("BIG_STEP_MODE"))
	{
		y1 = stepData.bigStepData[peakCnt - 1].av;
		y2 = stepData.bigStepData[peakCnt].av;
		y3 = stepData.bigStepData[peakCnt + 1].av;

		x1 = stepData.bigStepData[peakCnt - 1].code;
		x2 = stepData.bigStepData[peakCnt].code;
		x3 = stepData.bigStepData[peakCnt + 1].code;
	}
	else
	{
		y1 = stepData.miniStepData[peakCnt - 1].av;
		y2 = stepData.miniStepData[peakCnt].av;
		y3 = stepData.miniStepData[peakCnt + 1].av;

		x1 = stepData.miniStepData[peakCnt - 1].code;
		x2 = stepData.miniStepData[peakCnt].code;
		x3 = stepData.miniStepData[peakCnt + 1].code;
	}

	float a = ((y1 - y2) * (x2 - x3) - (y2 - y3) * (x1 - x2)) / ((x1 * x1 - x2 * x2) * (x2 - x3) - (x2 * x2 - x3 * x3) * (x1 - x2));
	float b = ((y1 - y2) * (x2 - x3) - a * (x1 * x1 - x2 * x2) * (x2 - x3)) / ((x1 - x2) * (x2 - x3));
	float c = y1 - a * x1 * x1 - b * x1;

	ret = (int)(b * (-1.0f) / (2 * a));
	*bestAv = (4 * a * c - b * b) / (4 * a);

	if (afMode == _T("BIG_STEP_MODE"))
	{
		if (ret < stepData.bigStepData[0].code)
			ret = stepData.bigStepData[0].code;
	}
	else
	{
		if (ret < stepData.miniStepData[0].code)
			ret = stepData.miniStepData[0].code;
	}

	if (afMode == _T("BIG_STEP_MODE"))
	{
		for (int i = 0; i<stepData.bigStepCnt - 1; i++)
		{
			if ((stepData.bigStepData[i].code <= ret) && (stepData.bigStepData[i + 1].code >= ret))
			{
				if ((stepData.bigStepData[i].code + stepCode / 3) > ret)
				{
					*startCode = stepData.bigStepData[i - 1].code;
					*endCode = stepData.bigStepData[i + 1].code;
				}
				else if ((stepData.bigStepData[i + 1].code - stepCode / 3) < ret)
				{
					*startCode = stepData.bigStepData[i].code;
					*endCode = stepData.bigStepData[i + 2].code;
				}
				else
				{
					*startCode = stepData.bigStepData[i].code;
					*endCode = stepData.bigStepData[i + 1].code;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i<stepData.miniStepCnt - 1; i++)
		{
			if ((stepData.miniStepData[i].code <= ret) && (stepData.miniStepData[i + 1].code >= ret))
			{
				if ((stepData.miniStepData[i].code + stepCode / 3) > ret)
				{
					*startCode = stepData.miniStepData[i - 1].code;
					*endCode = stepData.miniStepData[i + 1].code;
				}
				else if ((stepData.miniStepData[i + 1].code - stepCode / 3) < ret)
				{
					*startCode = stepData.miniStepData[i].code;
					*endCode = stepData.miniStepData[i + 2].code;
				}
				else
				{
					*startCode = stepData.miniStepData[i].code;
					*endCode = stepData.miniStepData[i + 1].code;
				}
			}
		}
	}

	return ret;
}


BOOL IsRGB_forQualCali(BYTE* rgb24, int imgWidth, int imgHeight)
{
	using namespace Halcon;
	BOOL ret = FALSE;

	DWORD size = imgWidth * imgHeight;
	BYTE* pBuffR = new BYTE[size];
	BYTE* pBuffG = new BYTE[size];
	BYTE* pBuffB = new BYTE[size];
	DWORD cnt = 0;
	for (DWORD i = 0; i < size * 3; i += 3)
	{
		pBuffR[cnt] = rgb24[i];
		pBuffG[cnt] = rgb24[i + 1];
		pBuffB[cnt] = rgb24[i + 2];
		cnt++;
	}
	Hobject ImageRGB, ImageR, ImageG, ImageB, ImageResultY, ImageResultU, ImageResultV;
	Hobject ImageEmphasize, Region, RegionOpening;

	HTuple Area, Row, Column;

	gen_image3(&ImageRGB, "byte", imgWidth, imgHeight, (long)pBuffR, (long)pBuffG, (long)pBuffB);
	decompose3(ImageRGB, &ImageR, &ImageG, &ImageB);
	trans_from_rgb(ImageR, ImageG, ImageB, &ImageResultY, &ImageResultU, &ImageResultV, "yuv");
	emphasize(ImageResultV, &ImageEmphasize, imgWidth, imgHeight, 10);
	threshold(ImageEmphasize, &Region, 0, 64);
	opening_circle(Region, &RegionOpening, 3.5);
	area_center(RegionOpening, &Area, &Row, &Column);

	if (0 != (Area < 30000))
		ret = FALSE;
	else
		ret = TRUE;

	ImageRGB.~Hobject();
	ImageR.~Hobject();
	ImageG.~Hobject();
	ImageB.~Hobject();
	ImageResultY.~Hobject();
	ImageResultU.~Hobject();
	ImageResultV.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();

	return ret;
}


int GetSquareChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint tagtPosi, CSize maskSize, int maskPhi, int* type, CPoint* roiCenterPosi, CString* msg)
{
	using namespace Halcon;

	// Local iconic variables 
	Hobject  GrayImage, Rectangle, ImageReduced;
	Hobject  ImageMedian, ImageEmphasize, RegionBlack, RegionErosionBlack;
	Hobject  RegionWhite, RegionErosionWhite, RegionUnion, ConnectedRegions;
	Hobject  RegionDilation, DestRegions, ObjectSelected, Contours;
	Hobject  XLDTrans, RegionA, RegionB, RegionIntersection;
	Hobject  SortedRegions;

	// Local control variables 
	HTuple  Width, Height;
	HTuple  TargetRow, TargetColumn, CenterRow, CenterColumn;
	HTuple  Number, PointRow, PointColumn, Row, Col, Mean, Min;
	HTuple  Max, OutRow, OutColumn, Distance1, Distance2;

	CString tmpStr = _T("");
	int ret = 0;

	gen_image1(&GrayImage, "byte", HTuple(imgSize.cx), HTuple(imgSize.cy), (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);

	TargetRow = HTuple(tagtPosi.y);
	TargetColumn = HTuple(tagtPosi.x);
	CenterRow = Height / 2;
	CenterColumn = Width / 2;
	gen_rectangle2(&Rectangle, TargetRow, TargetColumn, 0, HTuple(maskSize.cx), HTuple(maskSize.cy));
	reduce_domain(GrayImage, Rectangle, &ImageReduced);
	median_image(ImageReduced, &ImageMedian, "circle", 3, "mirrored");
	emphasize(ImageMedian, &ImageEmphasize, HTuple(maskSize.cx), HTuple(maskSize.cy), 10);

	threshold(ImageEmphasize, &RegionBlack, 0, 80);
	//erosion_rectangle1(RegionBlack, &RegionErosionBlack, 10, 10);
	erosion_rectangle1(RegionBlack, &RegionErosionBlack, 5, 5);

	threshold(ImageEmphasize, &RegionWhite, 175, 255);
	//erosion_rectangle1(RegionWhite, &RegionErosionWhite, 10, 10);
	erosion_rectangle1(RegionWhite, &RegionErosionWhite, 5, 5);

	union2(RegionErosionBlack, RegionErosionWhite, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	//dilation_rectangle1(ConnectedRegions, &RegionDilation, 12, 12);
	dilation_rectangle1(ConnectedRegions, &RegionDilation, 6, 6);

	select_region_point(RegionDilation, &DestRegions, TargetRow, TargetColumn);
	count_obj(DestRegions, &Number);
	if (0 != (Number<1))
	{
		tmpStr.Format(_T("Number=%d [>=1]"), Number[0].I());
		*msg = tmpStr;
		ret = 1;
		goto FreeMemory;
	}

	select_obj(DestRegions, &ObjectSelected, 1);
	gen_contour_region_xld(ObjectSelected, &Contours, "border");
	shape_trans_xld(Contours, &XLDTrans, "rectangle1");
	gen_region_contour_xld(Contours, &RegionA, "margin");
	gen_region_contour_xld(XLDTrans, &RegionB, "margin");
	intersection(RegionA, RegionB, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
	count_obj(SortedRegions, &Number);
	if (0 != (Number<4))
	{
		tmpStr.Format(_T("Number=%d [4]"), Number[0].I());
		*msg = tmpStr;
		ret = 2;
		goto FreeMemory;
	}

	PointRow = HTuple();
	PointColumn = HTuple();
	select_obj(SortedRegions, &ObjectSelected, 1);
	gen_contour_region_xld(ObjectSelected, &Contours, "border");
	get_contour_xld(Contours, &Row, &Col);
	tuple_mean(Row, &Mean);
	tuple_min(Col, &Min);
	PointRow.Append(Mean);
	PointColumn.Append(Min);

	select_obj(SortedRegions, &ObjectSelected, 2);
	gen_contour_region_xld(ObjectSelected, &Contours, "border");
	get_contour_xld(Contours, &Row, &Col);
	tuple_mean(Col, &Mean);
	tuple_min(Row, &Min);
	PointRow.Append(Min);
	PointColumn.Append(Mean);

	select_obj(SortedRegions, &ObjectSelected, 3);
	gen_contour_region_xld(ObjectSelected, &Contours, "border");
	get_contour_xld(Contours, &Row, &Col);
	tuple_mean(Col, &Mean);
	tuple_max(Row, &Max);
	PointRow.Append(Max);
	PointColumn.Append(Mean);

	select_obj(SortedRegions, &ObjectSelected, 4);
	gen_contour_region_xld(ObjectSelected, &Contours, "border");
	get_contour_xld(Contours, &Row, &Col);
	tuple_mean(Row, &Mean);
	tuple_max(Col, &Max);
	PointRow.Append(Mean);
	PointColumn.Append(Max);

	OutRow = HTuple();
	OutColumn = HTuple();
	if (0 != (HTuple(TargetRow<CenterRow).And(TargetColumn<CenterColumn)))
	{
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[0]), HTuple(PointColumn[0]), &Distance1);
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[3]), HTuple(PointColumn[3]), &Distance2);
		if (0 != (Distance1>Distance2))
		{
			OutRow.Append((HTuple(PointRow[2]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[2]) + HTuple(PointColumn[3])) / 2);

			OutRow.Append((HTuple(PointRow[1]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[1]) + HTuple(PointColumn[3])) / 2);
		}
		else
		{
			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[1])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[1])) / 2);

			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[2])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[2])) / 2);
		}
	}
	else if (0 != (HTuple(TargetRow<CenterRow).And(TargetColumn>CenterColumn)))
	{
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[1]), HTuple(PointColumn[1]), &Distance1);
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[2]), HTuple(PointColumn[2]), &Distance2);
		if (0 != (Distance1>Distance2))
		{
			OutRow.Append((HTuple(PointRow[2]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[2]) + HTuple(PointColumn[3])) / 2);

			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[2])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[2])) / 2);
		}
		else
		{
			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[1])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[1])) / 2);

			OutRow.Append((HTuple(PointRow[1]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[1]) + HTuple(PointColumn[3])) / 2);
		}
	}
	else if (0 != (HTuple(TargetRow>CenterRow).And(TargetColumn<CenterColumn)))
	{
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[1]), HTuple(PointColumn[1]), &Distance1);
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[2]), HTuple(PointColumn[2]), &Distance2);
		if (0 != (Distance1>Distance2))
		{
			OutRow.Append((HTuple(PointRow[2]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[2]) + HTuple(PointColumn[3])) / 2);

			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[2])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[2])) / 2);
		}
		else
		{
			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[1])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[1])) / 2);

			OutRow.Append((HTuple(PointRow[1]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[1]) + HTuple(PointColumn[3])) / 2);
		}
	}
	else
	{
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[0]), HTuple(PointColumn[0]), &Distance1);
		distance_pp(TargetRow, TargetColumn, HTuple(PointRow[3]), HTuple(PointColumn[3]), &Distance2);
		if (0 != (Distance1>Distance2))
		{
			OutRow.Append((HTuple(PointRow[2]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[2]) + HTuple(PointColumn[3])) / 2);

			OutRow.Append((HTuple(PointRow[1]) + HTuple(PointRow[3])) / 2);
			OutColumn.Append((HTuple(PointColumn[1]) + HTuple(PointColumn[3])) / 2);
		}
		else
		{
			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[1])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[1])) / 2);

			OutRow.Append((HTuple(PointRow[0]) + HTuple(PointRow[2])) / 2);
			OutColumn.Append((HTuple(PointColumn[0]) + HTuple(PointColumn[2])) / 2);
		}
	}

	type[0] = 0;
	roiCenterPosi[0].x = OutColumn[0].I();
	roiCenterPosi[0].y = OutRow[0].I();

	type[1] = 1;
	roiCenterPosi[1].x = OutColumn[1].I();
	roiCenterPosi[1].y = OutRow[1].I();

FreeMemory:
	GrayImage.~Hobject();
	Rectangle.~Hobject();
	ImageReduced.~Hobject();
	ImageMedian.~Hobject();
	ImageEmphasize.~Hobject();
	RegionBlack.~Hobject();
	RegionErosionBlack.~Hobject();
	RegionWhite.~Hobject();
	RegionErosionWhite.~Hobject();
	RegionUnion.~Hobject();
	ConnectedRegions.~Hobject();
	RegionDilation.~Hobject();
	DestRegions.~Hobject();
	ObjectSelected.~Hobject();
	Contours.~Hobject();
	XLDTrans.~Hobject();
	RegionA.~Hobject();
	RegionB.~Hobject();
	RegionIntersection.~Hobject();
	SortedRegions.~Hobject();

	return ret;
}


int GetNewSquareChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint* tagtPosi, int tagtPosiCnt, CPoint posiOffset, CPoint* roiCenterPosi, CString* msg)
{
	using namespace Halcon;
	if ((NULL == tagtPosi) || (NULL == roiCenterPosi))
	{
		*msg = _T("Memory error.");
		return 0;
	}

	// Local iconic variables 
	Hobject  GrayImage, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Region, ConnectedRegions, SelectedRegionsB;
	Hobject  ImageInvert, SelectedRegionsW, RegionUnionB, RegionUnionW;
	Hobject  RegionUnion;

	// Local control variables 
	HTuple  Width, Height;
	HTuple  CenterRow, CenterColumn, TargetRow, TargetColumn;
	HTuple  Area, Row, Column, NewtupleRow, NewtupleColumn;
	HTuple  Distance, Indices, MinPosiRow, MinPosiColumn, OutRow, OutColumn, OffsetRow;
	HTuple  OffsetColumn, Distance1, Distance2;

	CString tmpStr = _T("");
	int ret = 0;

	gen_image1(&GrayImage, "byte", HTuple(imgSize.cx), HTuple(imgSize.cy), (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);

	mean_image(GrayImage, &ImageMean, 201, 201);
	sub_image(GrayImage, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, 201, 201, 10);

	bin_threshold(ImageEmphasize, &Region);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegionsB, (HTuple("circularity").Append("area")), "and", (HTuple(0.8).Append(300)), (HTuple(1).Append(1200)));
	invert_image(ImageEmphasize, &ImageInvert);
	bin_threshold(ImageInvert, &Region);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegionsW, (HTuple("circularity").Append("area")), "and", (HTuple(0.8).Append(300)), (HTuple(1).Append(1200)));
	union1(SelectedRegionsB, &RegionUnionB);
	union1(SelectedRegionsW, &RegionUnionW);
	union2(RegionUnionB, RegionUnionW, &RegionUnion);

	connection(RegionUnion, &ConnectedRegions);
	area_center(ConnectedRegions, &Area, &Row, &Column);

	OutRow = HTuple();
	OutColumn = HTuple();
	CenterRow = Height / 2;
	CenterColumn = Width / 2;
	OffsetRow = HTuple(posiOffset.y);
	OffsetColumn = HTuple(posiOffset.x);
	for (int i = 0; i < tagtPosiCnt; i++)
	{
		TargetRow = HTuple(tagtPosi[i].y);
		TargetColumn = HTuple(tagtPosi[i].x);
		tuple_gen_const(Row.Num(), TargetRow, &NewtupleRow);
		tuple_gen_const(Column.Num(), TargetColumn, &NewtupleColumn);
		distance_pp(Row, Column, NewtupleRow, NewtupleColumn, &Distance);
		tuple_sort_index(Distance, &Indices);
		if (0 != (HTuple(Distance[HTuple(Indices[0])])>200))
			continue;

		MinPosiRow = Row[HTuple(Indices[0])];
		MinPosiColumn = Column[HTuple(Indices[0])];
		distance_pp(TargetRow, TargetColumn, CenterRow, CenterColumn, &Distance1);
		distance_pp(MinPosiRow, MinPosiColumn, CenterRow, CenterColumn, &Distance2);
		if (0 != (HTuple(TargetRow<CenterRow).And(TargetColumn<CenterColumn)))
		{
			if (0 != (Distance1>Distance2))
			{
				OutRow.Append(MinPosiRow - OffsetRow);
				OutColumn.Append(MinPosiColumn + OffsetColumn);

				OutRow.Append(MinPosiRow + OffsetColumn);
				OutColumn.Append(MinPosiColumn - OffsetRow);
			}
			else
			{
				OutRow.Append(MinPosiRow + OffsetRow);
				OutColumn.Append(MinPosiColumn - OffsetColumn);

				OutRow.Append(MinPosiRow + OffsetColumn);
				OutColumn.Append(MinPosiColumn + OffsetRow);
			}
		}
		else if (0 != (HTuple(TargetRow<CenterRow).And(TargetColumn>CenterColumn)))
		{
			if (0 != (Distance1>Distance2))
			{
				OutRow.Append(MinPosiRow - OffsetRow);
				OutColumn.Append(MinPosiColumn + OffsetColumn);

				OutRow.Append(MinPosiRow + OffsetColumn);
				OutColumn.Append(MinPosiColumn + OffsetRow);
			}
			else
			{
				OutRow.Append(MinPosiRow + OffsetRow);
				OutColumn.Append(MinPosiColumn - OffsetColumn);

				OutRow.Append(MinPosiRow - OffsetColumn);
				OutColumn.Append(MinPosiColumn - OffsetRow);
			}
		}
		else if (0 != (HTuple(TargetRow>CenterRow).And(TargetColumn<CenterColumn)))
		{
			if (0 != (Distance1>Distance2))
			{
				OutRow.Append(MinPosiRow + OffsetRow);
				OutColumn.Append(MinPosiColumn - OffsetColumn);

				OutRow.Append(MinPosiRow - OffsetColumn);
				OutColumn.Append(MinPosiColumn - OffsetRow);
			}
			else
			{
				OutRow.Append(MinPosiRow - OffsetRow);
				OutColumn.Append(MinPosiColumn + OffsetColumn);

				OutRow.Append(MinPosiRow + OffsetColumn);
				OutColumn.Append(MinPosiColumn + OffsetRow);
			}
		}
		else
		{
			if (0 != (Distance1>Distance2))
			{
				OutRow.Append(MinPosiRow + OffsetRow);
				OutColumn.Append(MinPosiColumn - OffsetColumn);

				OutRow.Append(MinPosiRow - OffsetColumn);
				OutColumn.Append(MinPosiColumn - OffsetRow);
			}
			else
			{
				OutRow.Append(MinPosiRow - OffsetRow);
				OutColumn.Append(MinPosiColumn + OffsetColumn);

				OutRow.Append(MinPosiRow - OffsetColumn);
				OutColumn.Append(MinPosiColumn - OffsetRow);
			}
		}
	}

	for (int i = 0; i < tagtPosiCnt * 2; i++)
	{
		roiCenterPosi[i].x = OutColumn[i].I();
		roiCenterPosi[i].y = OutRow[i].I();
	}

	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegionsB.~Hobject();
	ImageInvert.~Hobject();
	SelectedRegionsW.~Hobject();
	RegionUnionB.~Hobject();
	RegionUnionW.~Hobject();
	RegionUnion.~Hobject();

	return ret;
}


int GetSquareChartSfrValueForOPPO(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, int* roiType, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg)
{
	int ret = 0;
	CString tmpStr = _T("");

	OUTFORMAT rawOutFrm = (OUTFORMAT)rawFrm;
	for (int i = 0; i < 2; i++)
	{
		if ((roiCnPosi[i].x == 0) && (roiCnPosi[i].y == 0))
			continue;

		int roiWidth = ((roiSize.cx % 2 == 0) ? roiSize.cx : roiSize.cx - 1) + 4;
		int roiHeight = ((roiSize.cy % 2 == 0) ? roiSize.cy : roiSize.cy - 1) + 4;
		int roiStartX = roiCnPosi[i].x - roiWidth / 2;
		roiStartX = (roiStartX % 2 == 0) ? roiStartX : roiStartX + 1;
		int roiStartY = roiCnPosi[i].y - roiHeight / 2;
		roiStartY = (roiStartY % 2 == 0) ? roiStartY : roiStartY + 1;

		DWORD blockSize = roiWidth * roiHeight;
		USHORT* blockBuff = new USHORT[blockSize];
		memset(blockBuff, 0, blockSize);
		for (int p = 0; p < roiHeight; p++)
		{
			for (int k = 0; k < roiWidth; k++)
				blockBuff[p*roiWidth + k] = pRawData[(roiStartY + p) * imgSize.cx + (roiStartX + k)] - 16;
		}

#if 0
		USHORT* blockBuffRotation = new USHORT[blockSize];
		memset(blockBuffRotation, 0, blockSize);
		if (roiType[i] == 0)
		{
			if (blockBuff[0] > blockBuff[(roiHeight - 2) * roiWidth + roiWidth - 2])
			{
				for (int p = 0; p < roiWidth; p++)
				{
					USHORT* tmpHori = new USHORT[roiHeight];
					memset(tmpHori, 0, roiHeight);
					for (int k = 0; k < roiHeight; k++)
						tmpHori[k] = blockBuff[(roiHeight - k - 1) * roiWidth + p];
					for (int k = 0; k < roiHeight; k++)
						blockBuffRotation[p * roiHeight + k] = tmpHori[k];
					memcpy(&blockBuffRotation[p * roiHeight], tmpHori, roiHeight * sizeof(USHORT));
					if (tmpHori != NULL)
						delete[] tmpHori;
					tmpHori = NULL;
				}
				int tmp = roiWidth;
				roiWidth = roiHeight;
				roiHeight = tmp;
				switch (rawOutFrm)
				{
				case OUTFORMAT_GRBG:
					rawOutFrm = OUTFORMAT_BGGR;
					break;
				case OUTFORMAT_GBRG:
					rawOutFrm = OUTFORMAT_RGGB;
					break;
				case OUTFORMAT_BGGR:
					rawOutFrm = OUTFORMAT_GBRG;
					break;
				case OUTFORMAT_RGGB:
				default:
					rawOutFrm = OUTFORMAT_GRBG;
					break;
				}
			}
			else
			{
				for (int p = 0; p < roiWidth; p++)
				{
					USHORT* tmpHori = new USHORT[roiHeight];
					memset(tmpHori, 0, roiHeight);
					for (int k = 0; k < roiHeight; k++)
						tmpHori[k] = blockBuff[k * roiWidth + (roiWidth - 1 - p)];
					for (int k = 0; k < roiHeight; k++)
						blockBuffRotation[p * roiHeight + k] = tmpHori[k];
					memcpy(&blockBuffRotation[p * roiHeight], tmpHori, roiHeight * sizeof(USHORT));
					if (tmpHori != NULL)
						delete[] tmpHori;
					tmpHori = NULL;
				}
				int tmp = roiWidth;
				roiWidth = roiHeight;
				roiHeight = tmp;
				switch (rawOutFrm)
				{
				case OUTFORMAT_GRBG:
					rawOutFrm = OUTFORMAT_RGGB;
					break;
				case OUTFORMAT_GBRG:
					rawOutFrm = OUTFORMAT_BGGR;
					break;
				case OUTFORMAT_BGGR:
					rawOutFrm = OUTFORMAT_GRBG;
					break;
				case OUTFORMAT_RGGB:
				default:
					rawOutFrm = OUTFORMAT_GBRG;
					break;
				}
			}

			memset(blockBuff, 0, sizeof(blockSize));
			memcpy(blockBuff, blockBuffRotation, blockSize * sizeof(USHORT));
		}
		else
		{
			if (blockBuff[0] > blockBuff[(roiHeight - 2) * roiWidth + roiWidth - 2])
			{
				for (int p = 0; p < roiHeight; p++)
				{
					for (int k = 0; k < roiWidth; k++)
						blockBuffRotation[p * roiWidth + k] = blockBuff[p * roiWidth + (roiWidth - 1 - k)];
				}
				switch (rawOutFrm)
				{
				case OUTFORMAT_GRBG:
					rawOutFrm = OUTFORMAT_RGGB;
					break;
				case OUTFORMAT_GBRG:
					rawOutFrm = OUTFORMAT_BGGR;
					break;
				case OUTFORMAT_BGGR:
					rawOutFrm = OUTFORMAT_GBRG;
					break;
				case OUTFORMAT_RGGB:
				default:
					rawOutFrm = OUTFORMAT_GRBG;
					break;
				}

				memset(blockBuff, 0, sizeof(blockSize));
				memcpy(blockBuff, blockBuffRotation, blockSize * sizeof(USHORT));
			}
		}

		if (blockBuffRotation != NULL)
			delete[] blockBuffRotation;
		blockBuffRotation = NULL;
#endif

		switch (rawOutFrm)
		{
		case OUTFORMAT_GRBG:
		case OUTFORMAT_GBRG:
			for (int p = 2; p < roiHeight - 2; p++)
			{
				for (int k = 2; k < roiWidth - 2; k++)
				{
					if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
						;
					else
					{
						int absV = abs(blockBuff[(p - 1) * roiWidth + (k + 0)] - blockBuff[(p + 1) * roiWidth + (k + 0)]);
						int absH = abs(blockBuff[(p + 0) * roiWidth + (k - 1)] - blockBuff[(p + 0) * roiWidth + (k + 1)]);
						blockBuff[p * roiWidth + k] = (absV < absH) ? (blockBuff[(p - 1) * roiWidth + (k + 0)] + blockBuff[(p + 1) * roiWidth + (k + 0)]) / 2 : (blockBuff[(p + 0) * roiWidth + (k - 1)] + blockBuff[(p + 0) * roiWidth + (k + 1)]) / 2;
					}
				}
			}
			break;
		case OUTFORMAT_BGGR:
		case OUTFORMAT_RGGB:
		default:
			for (int p = 2; p < roiHeight-2; p++)
			{
				for (int k = 2; k < roiWidth - 2; k++)
				{
					if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
					{
						int absV = abs(blockBuff[(p - 1) * roiWidth + (k + 0)] - blockBuff[(p + 1) * roiWidth + (k + 0)]);
						int absH = abs(blockBuff[(p + 0) * roiWidth + (k - 1)] - blockBuff[(p + 0) * roiWidth + (k + 1)]);
						blockBuff[p * roiWidth + k] = (absV < absH) ? (blockBuff[(p - 1) * roiWidth + (k + 0)] + blockBuff[(p + 1) * roiWidth + (k + 0)]) / 2 : (blockBuff[(p + 0) * roiWidth + (k - 1)] + blockBuff[(p + 0) * roiWidth + (k + 1)]) / 2;
					}
				}
			}
			break;
		}

		DWORD blockFinalSize = (roiWidth - 4) * (roiHeight - 4);
		USHORT* blockFinalBuff = new USHORT[blockFinalSize];
		memset(blockFinalBuff, 0, blockFinalSize);
		DWORD cnt = 0;
		for (int p = 2; p < roiHeight - 2; p++)
		{
			for (int k = 2; k < roiWidth - 2; k++)
				blockFinalBuff[cnt++] = blockBuff[p * roiWidth + k];
		}

		if (blockBuff != NULL)
			delete[] blockBuff;
		blockBuff = NULL;

		int grayLT = blockFinalBuff[0];
		int grayRT = blockFinalBuff[(roiWidth - 4) - 1];
		int grayLB = blockFinalBuff[(roiWidth - 4) * ((roiHeight - 4) - 1)];
		int grayRB = blockFinalBuff[(roiWidth - 4) * (roiHeight - 4) - 1];
		if ((abs(grayLT - grayRB) < 50) || (abs(grayRT - grayLB) < 50))
		{
			tmpStr.Format(_T("ROI image error.%d %d %d %d"), grayLT, grayRT, grayLB, grayRB);
			*msg = tmpStr;
			ret = 1;
			if (blockFinalBuff != NULL)
				delete[] blockFinalBuff;
			blockFinalBuff = NULL;
			return ret;
		}

		CSFRData* sfrData = new CSFRData[256];
		sfrValue[i] = (float)SFRMatbmpTest(blockFinalBuff, freq, sfrData, roiWidth - 4, roiHeight - 4) + 0.01f;
		if ((sfrValue[i] < 0.0f) || (sfrValue[i] > 1.0f))
			sfrValue[i] = 0.0f;

		if (sfrData != NULL)
			delete[] sfrData;
		sfrData = NULL;

#if 0
		CString blockFinalFileName = _T("");
		if (roiType[i] == 0)
			blockFinalFileName.Format(_T("D:\\Data\\ROI_%02d_BlockH.raw"), roiNum);
		else
			blockFinalFileName.Format(_T("D:\\Data\\ROI_%02d_BlockV.raw"), roiNum);
		CStdioFile f;
		f.Open(blockFinalFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		f.Write(blockFinalBuff, blockFinalSize * sizeof(USHORT));
		f.Close();

		blockFinalFileName = _T("D:\\ROI_PosiInfo.txt");
		CStdioFile f1;
		f1.Open(blockFinalFileName, CFile::modeWrite | CFile::typeText);
		if (roiType[i] == 0)
			tmpStr.Format(_T("ROI%02d-H, %d-%d, %d-%d, %f, %f\r\n"), roiNum, roiCnPosi[i].x, roiCnPosi[i].y, roiSize.cx, roiSize.cy, freq, sfrValue[i]);
		else
			tmpStr.Format(_T("ROI%02d-V, %d-%d, %d-%d, %f, %f\r\n"), roiNum, roiCnPosi[i].x, roiCnPosi[i].y, roiSize.cx, roiSize.cy, freq, sfrValue[i]);
		f1.SeekToEnd();
		f1.WriteString(tmpStr);
		f1.Close();
#endif

		if (blockFinalBuff != NULL)
			delete[] blockFinalBuff;
		blockFinalBuff = NULL;
	}
	ret = 1;

	return ret;
}


int GetSquareChartSfrValueForWhiteBalance(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, int* roiType, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg)
{
	int ret = 0;
	CString tmpStr = _T("");

	OUTFORMAT rawOutFrm = (OUTFORMAT)rawFrm;
	for (int i = 0; i < 2; i++)
	{
		if ((roiCnPosi[i].x == 0) && (roiCnPosi[i].y == 0))
			continue;

		int roiWidth = ((roiSize.cx % 2 == 0) ? roiSize.cx : roiSize.cx - 1) + 4;
		int roiHeight = ((roiSize.cy % 2 == 0) ? roiSize.cy : roiSize.cy - 1) + 4;
		int roiStartX = roiCnPosi[i].x - roiWidth / 2;
		roiStartX = (roiStartX % 2 == 0) ? roiStartX : roiStartX + 1;
		int roiStartY = roiCnPosi[i].y - roiHeight / 2;
		roiStartY = (roiStartY % 2 == 0) ? roiStartY : roiStartY + 1;

		DWORD blockSize = roiWidth * roiHeight;
		BYTE* blockBuff = new BYTE[blockSize * 2];
		memset(blockBuff, 0, blockSize * 2);
		PCutImageBufferDll((BYTE*)pRawData, imgSize.cx, imgSize.cy, 2, roiStartX, roiStartY, roiSize.cx, roiSize.cy, blockBuff);

		int outformat = rawFrm;
		GetBayerTypeDll(rawFrm, roiStartX, roiStartY, outformat);

		BYTE* balanceBuff = new BYTE[blockSize * 2];
		memset(balanceBuff, 0, blockSize * 2);
		RawToGrayscaleImage_Raw10Dll((BYTE*)blockBuff, outformat, roiSize.cx, roiSize.cy, balanceBuff);

		if (blockBuff != NULL)
			delete[] blockBuff;
		blockBuff = NULL;

		DWORD s = roiWidth * roiHeight;
		USHORT *pROIBuffer = new USHORT[s];
		memset(pROIBuffer, 0, roiSize.cx * roiSize.cy * sizeof(USHORT));
		for (int y=0; y < roiSize.cy; y++)
		{
			for (int x=0; x < roiSize.cx; x++)
				pROIBuffer[y * roiSize.cx + x] = (balanceBuff[(y * roiSize.cx + x) * 2] | (balanceBuff[(y * roiSize.cx + x) * 2 + 1] << 8)) - 16;
		}
		
		if (balanceBuff != NULL)
			delete[] balanceBuff;
		balanceBuff = NULL;

		int grayLT = pROIBuffer[0];
		int grayRT = pROIBuffer[(roiWidth - 4) - 1];
		int grayLB = pROIBuffer[(roiWidth - 4) * ((roiHeight - 4) - 1)];
		int grayRB = pROIBuffer[(roiWidth - 4) * (roiHeight - 4) - 1];
		if ((abs(grayLT - grayRB) < 50) || (abs(grayRT - grayLB) < 50))
		{
			tmpStr.Format(_T("ROI image error.%d %d %d %d"), grayLT, grayRT, grayLB, grayRB);
			*msg = tmpStr;
			ret = 1;
			if (pROIBuffer != NULL)
				delete[] pROIBuffer;
			pROIBuffer = NULL;
			return ret;
		}

		CSFRData* sfrArray = new CSFRData[roiSize.cx];
		sfrValue[i] = (float)SFRMatbmpTest(pROIBuffer, freq, sfrArray, roiSize.cx, roiSize.cy);
		if (sfrValue[i] > 1.0f)
			sfrValue[i] = 1.0f;

		if (sfrValue[i] < 0.007f)
			sfrValue[i] = 0.01f;

		if (pROIBuffer != NULL)
			delete[] pROIBuffer;
		pROIBuffer = NULL;

		if (sfrArray != NULL)
			delete[] sfrArray;
		sfrArray = NULL;
	}
	ret = 1;

	return ret;
}


int GetXChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint tagtPosi, CSize maskSize, CPoint* roiCenterPosi, CString* msg)
{
#if 1	// VGA Enable
	using namespace Halcon;

	// Local iconic variables 
	Hobject  GrayImage, Rectangle, RegionFillUp, RegionTrans;
	Hobject  ImageReduced, ImageEmphasize, Region, TiledImage, ImageSub, Regions, RegionErosion;
	Hobject  ConnectedRegions, SelectedRegions, RegionDilation;
	Hobject  RegionTransA, RegionTransB, RegionMoved, ContoursA, ContoursB;
	Hobject  RegionA, RegionB, RegionIntersection, SortedRegions;
	Hobject  ObjectSelected, Contours;

	// Local control variables 
	HTuple  Width, Height, AbsoluteHisto, RelativeHisto, HistoSigma, MinThresh, MaxThresh;
	HTuple  TargetRow, TargetColumn, Number, PointRow;
	HTuple  PointColumn, Area, Row, Column, Col, RowA, Length;
	HTuple  ColA, ColB, RowB;

	CString tmpStr = _T("");
	int ret = 0;

	gen_image1(&GrayImage, "byte", HTuple(imgSize.cx), HTuple(imgSize.cy), (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);
	threshold(GrayImage, &Region, 40, 255);
	erosion_circle(Region, &RegionErosion, 6.5);
	fill_up(RegionErosion, &RegionFillUp);
	shape_trans(RegionFillUp, &RegionTrans, "convex");
	reduce_domain(GrayImage, RegionTrans, &GrayImage);

	TargetRow = HTuple(tagtPosi.y);
	TargetColumn = HTuple(tagtPosi.x);
	gen_rectangle1(&Rectangle, TargetRow - maskSize.cy, TargetColumn - maskSize.cx, TargetRow + maskSize.cy, TargetColumn + maskSize.cx);
	reduce_domain(GrayImage, Rectangle, &ImageReduced);
	emphasize(ImageReduced, &ImageEmphasize, 100, 100, 10);

	threshold(ImageEmphasize, &Region, 0, 80);
	connection(Region, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	count_obj(SelectedRegions, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d"), Number[0].I());
		*msg = tmpStr;
		ret = 1;
		goto FreeMemory;
	}
	erosion_circle(SelectedRegions, &RegionErosion, 1.5);
	shape_trans(RegionErosion, &RegionTransA, "rectangle2");
	shape_trans(RegionTransA, &RegionTransB, "rectangle1");
	gen_contour_region_xld(RegionTransA, &ContoursA, "border");
	gen_contour_region_xld(RegionTransB, &ContoursB, "border");
	gen_region_contour_xld(ContoursA, &RegionA, "margin");
	gen_region_contour_xld(ContoursB, &RegionB, "margin");
	intersection(RegionA, RegionB, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);

	PointRow = HTuple();
	PointColumn = HTuple();
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
	area_center(SortedRegions, &Area, &Row, &Column);
	tuple_length(Area, &Length);
	if (0 != (Length<4))
	{
		tmpStr.Format(_T("Length=%d"), Length[0].I());
		*msg = tmpStr;
		ret = 2;
		goto FreeMemory;
	}
	if (0 != (HTuple(Column[0])<HTuple(Column[1])))
	{
		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_min(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColB);
		tuple_min(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_max(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_max(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_min(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColB);
		tuple_max(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_min(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_max(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);
	}
	else
	{
		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_max(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);

		tuple_mean(Col, &ColB);
		tuple_min(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_max(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_min(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_min(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_min(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_max(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColB);
		tuple_max(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);
	}

	for (int i = 0; i < 4; i++)
	{
		roiCenterPosi[i].x = PointColumn[i].I();
		roiCenterPosi[i].y = PointRow[i].I();
	}

FreeMemory:
	GrayImage.~Hobject();
	Rectangle.~Hobject();
	RegionFillUp.~Hobject();
	RegionTrans.~Hobject();
	ImageReduced.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	TiledImage.~Hobject();
	ImageSub.~Hobject();
	Regions.~Hobject();
	RegionErosion.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionDilation.~Hobject();
	RegionTransA.~Hobject();
	RegionTransB.~Hobject();
	RegionMoved.~Hobject();
	ContoursA.~Hobject();
	ContoursB.~Hobject();
	RegionA.~Hobject();
	RegionB.~Hobject();
	RegionIntersection.~Hobject();
	SortedRegions.~Hobject();
	ObjectSelected.~Hobject();
	Contours.~Hobject();

	return ret;
#else
	using namespace Halcon;

	// Local iconic variables 
	Hobject  GrayImage, Rectangle;
	Hobject  ImageReduced, ImageEmphasize, Region, TiledImage, ImageSub, Regions, RegionErosion;
	Hobject  ConnectedRegions, SelectedRegions, RegionDilation;
	Hobject  RegionTrans, RegionMoved, ContoursA, ContoursB;
	Hobject  RegionA, RegionB, RegionIntersection, SortedRegions;
	Hobject  ObjectSelected, Contours;

	// Local control variables 
	HTuple  Width, Height, AbsoluteHisto, RelativeHisto, HistoSigma, MinThresh, MaxThresh;
	HTuple  TargetRow, TargetColumn, Number, PointRow;
	HTuple  PointColumn, Area, Row, Column, Col, RowA;
	HTuple  ColA, ColB, RowB;

	CString tmpStr = _T("");
	int ret = 0;

	gen_image1(&GrayImage, "byte", HTuple(imgSize.cx), HTuple(imgSize.cy), (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);

	TargetRow = HTuple(tagtPosi.y);
	TargetColumn = HTuple(tagtPosi.x);
	gen_rectangle1(&Rectangle, TargetRow - maskSize.cy, TargetColumn - maskSize.cx, TargetRow + maskSize.cy, TargetColumn + maskSize.cx);
	reduce_domain(GrayImage, Rectangle, &ImageReduced);
	gray_histo(ImageReduced, ImageReduced, &AbsoluteHisto, &RelativeHisto);
	HistoSigma = 1;
	histo_to_thresh(AbsoluteHisto, HistoSigma, &MinThresh, &MaxThresh);
	while (0 != 1)
	{
		if (0 != ((MinThresh.Num()) <= 2))
			break;
		else
		{
			HistoSigma += 1;
			histo_to_thresh(AbsoluteHisto, HistoSigma, &MinThresh, &MaxThresh);
		}
	}

	threshold(ImageReduced, &Region, 0, HTuple(MaxThresh[0]));
	connection(Region, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	count_obj(SelectedRegions, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d"), Number[0].I());
		*msg = tmpStr;
		ret = 1;
		goto FreeMemory;
	}
	area_center(SelectedRegions, &Area, &Row, &Column);
	if (0 != (Area < 40000))
	{
		tmpStr.Format(_T("Area=%d"), Area[0].I());
		*msg = tmpStr;
		ret = 2;
		goto FreeMemory;
	}
	dilation_circle(SelectedRegions, &RegionDilation, 6.5);
	shape_trans(RegionDilation, &RegionTrans, "rectangle2");
	move_region(RegionTrans, &RegionMoved, -1, -1);

	shape_trans(RegionMoved, &RegionTrans, "rectangle1");
	gen_contour_region_xld(RegionMoved, &ContoursA, "border");
	gen_contour_region_xld(RegionTrans, &ContoursB, "border");
	gen_region_contour_xld(ContoursA, &RegionA, "margin");
	gen_region_contour_xld(ContoursB, &RegionB, "margin");
	intersection(RegionA, RegionB, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	
	PointRow = HTuple();
	PointColumn = HTuple();
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
	area_center(SortedRegions, &Area, &Row, &Column);
	if (0 != (HTuple(Column[0])<HTuple(Column[1])))
	{
		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_min(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColB);
		tuple_min(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_max(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_max(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_min(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColB);
		tuple_max(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_min(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_max(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);
	}
	else
	{
		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_max(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);

		tuple_mean(Col, &ColB);
		tuple_min(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_max(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_min(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 2);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColA);
		tuple_min(Row, &RowA);

		select_obj(SortedRegions, &ObjectSelected, 4);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowB);
		tuple_min(Col, &ColB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);

		select_obj(SortedRegions, &ObjectSelected, 1);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Row, &RowA);
		tuple_max(Col, &ColA);

		select_obj(SortedRegions, &ObjectSelected, 3);
		gen_contour_region_xld(ObjectSelected, &Contours, "border");
		get_contour_xld(Contours, &Row, &Col);
		tuple_mean(Col, &ColB);
		tuple_max(Row, &RowB);
		PointRow.Append((RowA + RowB) / 2);
		PointColumn.Append((ColA + ColB) / 2);
	}

	for (int i = 0; i < 4; i++)
	{
		roiCenterPosi[i].x = PointColumn[i].I();
		roiCenterPosi[i].y = PointRow[i].I();
	}

FreeMemory:
	GrayImage.~Hobject();
	Rectangle.~Hobject();
	ImageReduced.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	TiledImage.~Hobject();
	ImageSub.~Hobject();
	Regions.~Hobject();
	RegionErosion.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionDilation.~Hobject();
	RegionTrans.~Hobject();
	RegionMoved.~Hobject();
	ContoursA.~Hobject();
	ContoursB.~Hobject();
	RegionA.~Hobject();
	RegionB.~Hobject();
	RegionIntersection.~Hobject();
	SortedRegions.~Hobject();
	ObjectSelected.~Hobject();
	Contours.~Hobject();

	return ret;
#endif
}


int GetXChartSfrValue(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg)
{
	int ret = 0;
	CString tmpStr = _T("");

	OUTFORMAT rawOutFrm = (OUTFORMAT)rawFrm;
	for (int i = 0; i < 4; i++)
	{
		if ((roiCnPosi[i].x == 0) && (roiCnPosi[i].y == 0))
			continue;
#if 0
		if (i == 0)
		{
			roiCnPosi[i].y += 8;
		}
		if (i == 1)
		{
			roiCnPosi[i].y -= 8;
		}
		if (i == 2)
		{
			roiCnPosi[i].x += 8;
		}
		if (i == 3)
		{
			roiCnPosi[i].x -= 8;
		}
#endif
		int roiWidth = ((roiSize.cx % 2 == 0) ? roiSize.cx : roiSize.cx - 1) + 4;
		int roiHeight = ((roiSize.cy % 2 == 0) ? roiSize.cy : roiSize.cy - 1) + 4;
		int roiStartX = roiCnPosi[i].x - roiWidth / 2;
		roiStartX = (roiStartX % 2 == 0) ? roiStartX : roiStartX + 1;
		if (roiStartX < 0)
			roiStartX = 0;
		int roiStartY = roiCnPosi[i].y - roiHeight / 2;
		roiStartY = (roiStartY % 2 == 0) ? roiStartY : roiStartY + 1;
		if (roiStartY < 0)
			roiStartY = 0;

		if ((roiStartX + roiWidth) > imgSize.cx)
			roiWidth = imgSize.cx - roiStartX;
		if ((roiStartY + roiHeight) > imgSize.cy)
			roiHeight = imgSize.cy - roiStartY;

		DWORD blockSize = roiWidth * roiHeight;
		USHORT* blockBuff = new USHORT[blockSize];
		memset(blockBuff, 0, blockSize);
		for (int p = 0; p < roiHeight; p++)
		{
			for (int k = 0; k < roiWidth; k++)
				blockBuff[p*roiWidth + k] = pRawData[(roiStartY + p) * imgSize.cx + (roiStartX + k)];
		}

#if 0
		USHORT* blockBuffRotation = new USHORT[blockSize];
		memset(blockBuffRotation, 0, blockSize);
		if (roiType[i] == 0)
		{
			if (blockBuff[0] > blockBuff[(roiHeight - 2) * roiWidth + roiWidth - 2])
			{
				for (int p = 0; p < roiWidth; p++)
				{
					USHORT* tmpHori = new USHORT[roiHeight];
					memset(tmpHori, 0, roiHeight);
					for (int k = 0; k < roiHeight; k++)
						tmpHori[k] = blockBuff[(roiHeight - k - 1) * roiWidth + p];
					for (int k = 0; k < roiHeight; k++)
						blockBuffRotation[p * roiHeight + k] = tmpHori[k];
					memcpy(&blockBuffRotation[p * roiHeight], tmpHori, roiHeight * sizeof(USHORT));
					if (tmpHori != NULL)
						delete[] tmpHori;
					tmpHori = NULL;
				}
				int tmp = roiWidth;
				roiWidth = roiHeight;
				roiHeight = tmp;
				switch (rawOutFrm)
				{
				case OUTFORMAT_GRBG:
					rawOutFrm = OUTFORMAT_BGGR;
					break;
				case OUTFORMAT_GBRG:
					rawOutFrm = OUTFORMAT_RGGB;
					break;
				case OUTFORMAT_BGGR:
					rawOutFrm = OUTFORMAT_GBRG;
					break;
				case OUTFORMAT_RGGB:
				default:
					rawOutFrm = OUTFORMAT_GRBG;
					break;
				}
			}
			else
			{
				for (int p = 0; p < roiWidth; p++)
				{
					USHORT* tmpHori = new USHORT[roiHeight];
					memset(tmpHori, 0, roiHeight);
					for (int k = 0; k < roiHeight; k++)
						tmpHori[k] = blockBuff[k * roiWidth + (roiWidth - 1 - p)];
					for (int k = 0; k < roiHeight; k++)
						blockBuffRotation[p * roiHeight + k] = tmpHori[k];
					memcpy(&blockBuffRotation[p * roiHeight], tmpHori, roiHeight * sizeof(USHORT));
					if (tmpHori != NULL)
						delete[] tmpHori;
					tmpHori = NULL;
				}
				int tmp = roiWidth;
				roiWidth = roiHeight;
				roiHeight = tmp;
				switch (rawOutFrm)
				{
				case OUTFORMAT_GRBG:
					rawOutFrm = OUTFORMAT_RGGB;
					break;
				case OUTFORMAT_GBRG:
					rawOutFrm = OUTFORMAT_BGGR;
					break;
				case OUTFORMAT_BGGR:
					rawOutFrm = OUTFORMAT_GRBG;
					break;
				case OUTFORMAT_RGGB:
				default:
					rawOutFrm = OUTFORMAT_GBRG;
					break;
				}
			}

			memset(blockBuff, 0, sizeof(blockSize));
			memcpy(blockBuff, blockBuffRotation, blockSize * sizeof(USHORT));
		}
		else
		{
			if (blockBuff[0] > blockBuff[(roiHeight - 2) * roiWidth + roiWidth - 2])
			{
				for (int p = 0; p < roiHeight; p++)
				{
					for (int k = 0; k < roiWidth; k++)
						blockBuffRotation[p * roiWidth + k] = blockBuff[p * roiWidth + (roiWidth - 1 - k)];
				}
				switch (rawOutFrm)
				{
				case OUTFORMAT_GRBG:
					rawOutFrm = OUTFORMAT_RGGB;
					break;
				case OUTFORMAT_GBRG:
					rawOutFrm = OUTFORMAT_BGGR;
					break;
				case OUTFORMAT_BGGR:
					rawOutFrm = OUTFORMAT_GBRG;
					break;
				case OUTFORMAT_RGGB:
				default:
					rawOutFrm = OUTFORMAT_GRBG;
					break;
				}

				memset(blockBuff, 0, sizeof(blockSize));
				memcpy(blockBuff, blockBuffRotation, blockSize * sizeof(USHORT));
			}
		}

		if (blockBuffRotation != NULL)
			delete[] blockBuffRotation;
		blockBuffRotation = NULL;
#endif
#if 1
		switch (rawOutFrm)
		{
		case OUTFORMAT_GRBG:
		case OUTFORMAT_GBRG:
			for (int p = 2; p < roiHeight - 2; p++)
			{
				for (int k = 2; k < roiWidth - 2; k++)
				{
					if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
						;
					else
					{
						int absV = abs(blockBuff[(p - 1) * roiWidth + (k + 0)] - blockBuff[(p + 1) * roiWidth + (k + 0)]);
						int absH = abs(blockBuff[(p + 0) * roiWidth + (k - 1)] - blockBuff[(p + 0) * roiWidth + (k + 1)]);
						blockBuff[p * roiWidth + k] = (absV < absH) ? (blockBuff[(p - 1) * roiWidth + (k + 0)] + blockBuff[(p + 1) * roiWidth + (k + 0)]) / 2 : (blockBuff[(p + 0) * roiWidth + (k - 1)] + blockBuff[(p + 0) * roiWidth + (k + 1)]) / 2;
					}
				}
			}
			break;
		case OUTFORMAT_BGGR:
		case OUTFORMAT_RGGB:
		default:
			for (int p = 2; p < roiHeight - 2; p++)
			{
				for (int k = 2; k < roiWidth - 2; k++)
				{
					if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
					{
						int absV = abs(blockBuff[(p - 1) * roiWidth + (k + 0)] - blockBuff[(p + 1) * roiWidth + (k + 0)]);
						int absH = abs(blockBuff[(p + 0) * roiWidth + (k - 1)] - blockBuff[(p + 0) * roiWidth + (k + 1)]);
						blockBuff[p * roiWidth + k] = (absV < absH) ? (blockBuff[(p - 1) * roiWidth + (k + 0)] + blockBuff[(p + 1) * roiWidth + (k + 0)]) / 2 : (blockBuff[(p + 0) * roiWidth + (k - 1)] + blockBuff[(p + 0) * roiWidth + (k + 1)]) / 2;
					}
				}
			}
			break;
		}
#endif
		DWORD blockFinalSize = (roiWidth - 4) * (roiHeight - 4);
		USHORT* blockFinalBuff = new USHORT[blockFinalSize];
		memset(blockFinalBuff, 0, blockFinalSize);
		DWORD cnt = 0;
		for (int p = 2; p < roiHeight - 2; p++)
		{
			for (int k = 2; k < roiWidth - 2; k++)
				blockFinalBuff[cnt++] = blockBuff[p * roiWidth + k];
		}

		if (blockBuff != NULL)
			delete[] blockBuff;
		blockBuff = NULL;

		int grayLT = blockFinalBuff[0];
		int grayRT = blockFinalBuff[(roiWidth - 4) - 1];
		int grayLB = blockFinalBuff[(roiWidth - 4) * ((roiHeight - 4) -1)];
		int grayRB = blockFinalBuff[(roiWidth - 4) * (roiHeight - 4) - 1];
		if ((abs(grayLT - grayRB) < 100) || (abs(grayRT - grayLB) < 100))
		{
			tmpStr.Format(_T("ROI image error.%d %d %d %d"), grayLT, grayRT, grayLB, grayRB);
			*msg = tmpStr;
			ret = 1;
			if (blockFinalBuff != NULL)
				delete[] blockFinalBuff;
			blockFinalBuff = NULL;
			//return ret;
			continue;
		}

		CSFRData* sfrData = new CSFRData[256];
		sfrValue[i] = (float)SFRMatbmpTest(blockFinalBuff, freq, sfrData, roiWidth - 4, roiHeight - 4);
		if ((sfrValue[i] < 0.0f) || (sfrValue[i] > 1.0f))
			sfrValue[i] = 0.0f;

		if (sfrData != NULL)
			delete[] sfrData;
		sfrData = NULL;

#if 1
		if (roiNum == 0)
		{
			USHORT* demosaicBuff = new USHORT[imgSize.cx * imgSize.cy];
			memcpy(demosaicBuff, pRawData, imgSize.cx * imgSize.cy * sizeof(USHORT));
			switch (rawOutFrm)
			{
			case OUTFORMAT_GRBG:
			case OUTFORMAT_GBRG:
				for (int p = 2; p < imgSize.cy - 2; p++)
				{
					for (int k = 2; k < imgSize.cx - 2; k++)
					{
						if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
							;
						else
						{
							int absV = abs(demosaicBuff[(p - 1) * imgSize.cx + (k + 0)] - demosaicBuff[(p + 1) * imgSize.cx + (k + 0)]);
							int absH = abs(demosaicBuff[(p + 0) * imgSize.cx + (k - 1)] - demosaicBuff[(p + 0) * imgSize.cx + (k + 1)]);
							demosaicBuff[p * imgSize.cx + k] = (absV < absH) ? (demosaicBuff[(p - 1) * imgSize.cx + (k + 0)] + demosaicBuff[(p + 1) * imgSize.cx + (k + 0)]) / 2 : (demosaicBuff[(p + 0) * imgSize.cx + (k - 1)] + demosaicBuff[(p + 0) * imgSize.cx + (k + 1)]) / 2;
						}
					}
				}
				break;
			case OUTFORMAT_BGGR:
			case OUTFORMAT_RGGB:
			default:
				for (int p = 2; p < imgSize.cy - 2; p++)
				{
					for (int k = 2; k < imgSize.cx - 2; k++)
					{
						if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
						{
							int absV = abs(demosaicBuff[(p - 1) * imgSize.cx + (k + 0)] - demosaicBuff[(p + 1) * imgSize.cx + (k + 0)]);
							int absH = abs(demosaicBuff[(p + 0) * imgSize.cx + (k - 1)] - demosaicBuff[(p + 0) * imgSize.cx + (k + 1)]);
							demosaicBuff[p * imgSize.cx + k] = (absV < absH) ? (demosaicBuff[(p - 1) * imgSize.cx + (k + 0)] + demosaicBuff[(p + 1) * imgSize.cx + (k + 0)]) / 2 : (demosaicBuff[(p + 0) * imgSize.cx + (k - 1)] + demosaicBuff[(p + 0) * imgSize.cx + (k + 1)]) / 2;
						}
					}
				}
				break;
			}

			CStdioFile balanceRawFile;
			balanceRawFile.Open(_T("D:\\BalanceRaw.raw"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			balanceRawFile.Write(demosaicBuff, imgSize.cx * imgSize.cy * 2);
			balanceRawFile.Close();

			if (demosaicBuff != NULL)
				delete[] demosaicBuff;
			demosaicBuff = NULL;
		}

		CString blockFinalFileName = _T("");
		if (i == 0)
			blockFinalFileName.Format(_T("D:\\ROI_%02d_H_TOP.raw"), roiNum);
		else if (i == 1)
			blockFinalFileName.Format(_T("D:\\ROI_%02d_H_BOTTOM.raw"), roiNum);
		else if (i == 2)
			blockFinalFileName.Format(_T("D:\\ROI_%02d_V_LEFT.raw"), roiNum);
		else if (i == 3)
			blockFinalFileName.Format(_T("D:\\ROI_%02d_V_RIGHT.raw"), roiNum);
		CStdioFile f;
		f.Open(blockFinalFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		f.Write(blockFinalBuff, blockFinalSize * sizeof(USHORT));
		f.Close();

		//blockFinalFileName = _T("D:\\ROI_PosiInfo.txt");
		//CStdioFile f1;
		//f1.Open(blockFinalFileName, CFile::modeWrite | CFile::typeText);
		//if (i == 0)
		//	tmpStr.Format(_T("ROI%02d-H-TOP, %d-%d, %d-%d, %f, %f\r\n"), roiNum, roiCnPosi[i].x, roiCnPosi[i].y, roiSize.cx, roiSize.cy, freq, sfrValue[i]);
		//else if (i == 1)
		//	tmpStr.Format(_T("ROI%02d-H-BOTTOM, %d-%d, %d-%d, %f, %f\r\n"), roiNum, roiCnPosi[i].x, roiCnPosi[i].y, roiSize.cx, roiSize.cy, freq, sfrValue[i]);
		//else if (i == 2)
		//	tmpStr.Format(_T("ROI%02d-V-LEFT, %d-%d, %d-%d, %f, %f\r\n"), roiNum, roiCnPosi[i].x, roiCnPosi[i].y, roiSize.cx, roiSize.cy, freq, sfrValue[i]);
		//else if (i == 3)
		//	tmpStr.Format(_T("ROI%02d-V-RIGHT, %d-%d, %d-%d, %f, %f\r\n"), roiNum, roiCnPosi[i].x, roiCnPosi[i].y, roiSize.cx, roiSize.cy, freq, sfrValue[i]);
		//f1.SeekToEnd();
		//f1.WriteString(tmpStr);
		//f1.Close();
#endif

		if (blockFinalBuff != NULL)
			delete[] blockFinalBuff;
		blockFinalBuff = NULL;
	}
	ret = 1;

	return ret;
}


int GetMotoChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CSize maskSize, int maskOffsetX, CPoint* roiCenterPosi, CString* msg)
{
	using namespace Halcon;

	// Local iconic variables 
	Hobject  GrayImage, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Region, RegionOpening, Circle, RegionIntersection;
	Hobject  ConnectedRegions, SelectedRegions, Regions, RegionUnionCrossLine;
	Hobject  RegionFillUp, RegionUnionAll, RegionTrans, RegionDilation;
	Hobject  RegionIntersectionCenter;
	Hobject  RegionUnion, RegionDifferenceB, Rectangle, SortedRegions;


	// Local control variables 
	HTuple  ImageFiles, Index, Width, Height, WindowHandle;
	HTuple  SecondsA, Number, Area, Row, Column, OffsetLine;
	HTuple  RowCenter, ColumnCenter, OffsetX, SecondsB, Diff;

	CString tmpStr = _T("");
	int ret = 0;

	gen_image1(&GrayImage, "byte", HTuple(imgSize.cx), HTuple(imgSize.cy), (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);
	zoom_image_size(GrayImage, &GrayImage, Width / 4, Height / 4, "constant");
	Width = Width / 4;
	Height = Height / 4;
	median_image(GrayImage, &GrayImage, "circle", 1, "mirrored");

	mean_image(GrayImage, &ImageMean, Width / 64, Height / 64);
	sub_image(GrayImage, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 10);

	threshold(ImageEmphasize, &Region, 0, 80);
	opening_circle(Region, &RegionOpening, 1.5);
	gen_circle(&Circle, Height / 2, Width / 2, 100);
	intersection(RegionOpening, Circle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	fill_up(ConnectedRegions, &RegionFillUp);
	select_shape_std(RegionFillUp, &SelectedRegions, "max_area", 70);
	count_obj(SelectedRegions, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d [1]"), Number[0].I());
		*msg = tmpStr;
		ret = 1;
		goto FreeMemory;
	}
	area_center(SelectedRegions, &Area, &Row, &Column);

	gen_region_hline(&Regions, HTuple(HTuple(0).Rad()).Concat(HTuple(90).Rad()), Column.Concat(Row));
	union1(Regions, &RegionUnionCrossLine);

	threshold(ImageEmphasize, &Region, 0, 124);
	opening_circle(Region, &RegionOpening, 2.5);
	fill_up(RegionOpening, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, ((HTuple("area").Append("width")).Append("height")), "and", ((HTuple(1500).Append(15)).Append(30)), ((HTuple(3000).Append(80)).Append(120)));
	union1(SelectedRegions, &RegionUnionAll);

	OffsetX = maskOffsetX / 4;
	shape_trans(RegionUnionCrossLine, &RegionTrans, "inner_circle");
	dilation_rectangle1(RegionTrans, &RegionDilation, 60, 80);
	intersection(RegionDilation, RegionUnionAll, &RegionIntersectionCenter);
	count_obj(RegionIntersectionCenter, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d [1]"), Number[0].I());
		*msg = tmpStr;
		ret = 2;
		goto FreeMemory;
	}
	area_center(RegionIntersectionCenter, &Area, &RowCenter, &ColumnCenter);
	roiCenterPosi[0].x = (ColumnCenter - OffsetX)[0].I() * 4;
	roiCenterPosi[0].y = RowCenter[0].I() * 4;

	union2(RegionUnionAll, RegionUnionCrossLine, &RegionUnion);
	connection(RegionUnion, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
#if 0
	dilation_circle(RegionIntersectionCenter, &RegionDilation, 27.5);
	difference(SelectedRegions, RegionDilation, &RegionDifference);
	opening_circle(RegionDifference, &RegionOpening, 3.5);
	connection(RegionOpening, &ConnectedRegions);
	count_obj(ConnectedRegions, &Number);
	if (0 != (Number != 6))
	{
		tmpStr.Format(_T("Number=%d [6]"), Number[0].I());
		*msg = tmpStr;
		ret = 3;
		goto FreeMemory;
	}
	area_center(ConnectedRegions, &Area, &Row, &Column);
	roiCenterPosi[13].x = (Column[0] - OffsetX)[0].I() * 4;
	roiCenterPosi[13].y = Row[0].I() * 4;
	roiCenterPosi[14].x = (Column[5] - OffsetX)[0].I() * 4;
	roiCenterPosi[14].y = Row[5].I() * 4;

	gen_empty_obj(&EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 2);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 3);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 4);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	select_obj(ConnectedRegions, &ObjectSelected, 5);
	concat_obj(EmptyObject, ObjectSelected, &EmptyObject);
	sort_region(EmptyObject, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area, &Row, &Column);
	roiCenterPosi[15].x = (Column[0] - OffsetX)[0].I() * 4;
	roiCenterPosi[15].y = Row[0].I() * 4;
	roiCenterPosi[16].x = (Column[1] - OffsetX)[0].I() * 4;
	roiCenterPosi[16].y = Row[1].I() * 4;
	roiCenterPosi[17].x = (Column[3] + OffsetX)[0].I() * 4;
	roiCenterPosi[17].y = Row[3].I() * 4;
	roiCenterPosi[18].x = (Column[2] + OffsetX)[0].I() * 4;
	roiCenterPosi[18].y = Row[2].I() * 4;
#endif
	difference(RegionUnionAll, SelectedRegions, &RegionDifferenceB);
	gen_rectangle1(&Rectangle, 20, 20, RowCenter - 60, ColumnCenter - 60);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		tmpStr.Format(_T("Number=%d [3]"), Number[0].I());
		*msg = tmpStr;
		ret = 4;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	roiCenterPosi[1].x = (Column[0] - OffsetX)[0].I() * 4;
	roiCenterPosi[1].y = Row[0].I() * 4;
	roiCenterPosi[5].x = (Column[1] - OffsetX)[0].I() * 4;
	roiCenterPosi[5].y = Row[1].I() * 4;
	roiCenterPosi[9].x = (Column[2] - OffsetX)[0].I() * 4;
	roiCenterPosi[9].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, 20, ColumnCenter + 60, RowCenter - 60, (Width - 1) - 20);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		tmpStr.Format(_T("Number=%d [3]"), Number[0].I());
		*msg = tmpStr;
		ret = 5;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	roiCenterPosi[10].x = (Column[0] + OffsetX)[0].I() * 4;
	roiCenterPosi[10].y = Row[0].I() * 4;
	roiCenterPosi[6].x = (Column[1] + OffsetX)[0].I() * 4;
	roiCenterPosi[6].y = Row[1].I() * 4;
	roiCenterPosi[2].x = (Column[2] + OffsetX)[0].I() * 4;
	roiCenterPosi[2].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, RowCenter + 60, 20, (Height - 1) - 20, ColumnCenter - 60);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		tmpStr.Format(_T("Number=%d [3]"), Number[0].I());
		*msg = tmpStr;
		ret = 6;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	roiCenterPosi[3].x = (Column[0] - OffsetX)[0].I() * 4;
	roiCenterPosi[3].y = Row[0].I() * 4;
	roiCenterPosi[7].x = (Column[1] - OffsetX)[0].I() * 4;
	roiCenterPosi[7].y = Row[1].I() * 4;
	roiCenterPosi[11].x = (Column[2] - OffsetX)[0].I() * 4;
	roiCenterPosi[11].y = Row[2].I() * 4;

	gen_rectangle1(&Rectangle, RowCenter + 60, ColumnCenter + 60, (Height - 1) - 20, (Width - 1) - 20);
	intersection(RegionDifferenceB, Rectangle, &RegionIntersection);
	connection(RegionIntersection, &ConnectedRegions);
	sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &Number);
	if (0 != (Number != 3))
	{
		tmpStr.Format(_T("Number=%d [3]"), Number[0].I());
		*msg = tmpStr;
		ret = 7;
		goto FreeMemory;
	}
	area_center(SortedRegions, &Area, &Row, &Column);
	roiCenterPosi[12].x = (Column[0] + OffsetX)[0].I() * 4;
	roiCenterPosi[12].y = Row[0].I() * 4;
	roiCenterPosi[8].x = (Column[1] + OffsetX)[0].I() * 4;
	roiCenterPosi[8].y = Row[1].I() * 4;
	roiCenterPosi[4].x = (Column[2] + OffsetX)[0].I() * 4;
	roiCenterPosi[4].y = Row[2].I() * 4;

FreeMemory:
	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionOpening.~Hobject();
	Circle.~Hobject();
	RegionIntersection.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	Regions.~Hobject();
	RegionUnionCrossLine.~Hobject();
	RegionFillUp.~Hobject();
	RegionUnionAll.~Hobject();
	RegionTrans.~Hobject();
	RegionDilation.~Hobject();
	RegionIntersectionCenter.~Hobject();
	RegionUnion.~Hobject();
	//RegionDifference.~Hobject();
	//EmptyObject.~Hobject();
	//ObjectSelected.~Hobject();
	SortedRegions.~Hobject();
	RegionDifferenceB.~Hobject();
	Rectangle.~Hobject();

	return ret;
}


int GetMotoChartSfrValue(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg)
{
	int ret = 0;

	return ret;
}


int GetHwMmiTargetPosi(BYTE* pGrayBuff, CSize imgSize, CPoint* roiCnPosi, CString* msg)
{
	using namespace Halcon;

	CString tmpStr = _T("");
	int ret = 0;

	// Local iconic variables 
	Hobject  GrayImage, ImageEmphasize;
	Hobject  Region, RegionClosing, RegionFillUp, ConnectedRegions;
	Hobject  DestRegions, RegionTransUp, RegionTransDown, RegionLines;
	Hobject  SelectedRegions, RegionDilation, RegionUnionUp;
	Hobject  RegionUnionDown;


	// Local control variables 
	HTuple  Width, Height, WindowHandle, RowTargetPosi;
	HTuple  ColumnTargetPosi, Area, Row, Column, Number;

	gen_image1(&GrayImage, "byte", imgSize.cx, imgSize.cy, (long)pGrayBuff);
	zoom_image_size(GrayImage, &GrayImage, 3968, 2976, "bilinear");
	get_image_size(GrayImage, &Width, &Height);

	RowTargetPosi.Reset();
	RowTargetPosi.Append(Height / 5);
	RowTargetPosi.Append((Height * 4) / 5);
	ColumnTargetPosi.Reset();
	ColumnTargetPosi.Append(Width / 2);
	ColumnTargetPosi.Append(Width / 2);

	emphasize(GrayImage, &ImageEmphasize, 51, 51, 10);
	threshold(ImageEmphasize, &Region, 80, 255);
	closing_circle(Region, &RegionClosing, 13.5);
	fill_up(RegionClosing, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);

	select_region_point(ConnectedRegions, &DestRegions, HTuple(RowTargetPosi[0]), HTuple(ColumnTargetPosi[0]));
	shape_trans(DestRegions, &RegionTransUp, "convex");
	count_obj(RegionTransUp, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d [1]"), Number[0].I());
		*msg = tmpStr;
		ret = 1;
		goto FreeMemory;
	}

	select_region_point(ConnectedRegions, &DestRegions, HTuple(RowTargetPosi[1]), HTuple(ColumnTargetPosi[1]));
	shape_trans(DestRegions, &RegionTransDown, "convex");
	count_obj(RegionTransDown, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d [1]"), Number[0].I());
		*msg = tmpStr;
		ret = 2;
		goto FreeMemory;
	}

	split_skeleton_region(RegionTransUp, &RegionLines, 3);
	select_shape(RegionLines, &SelectedRegions, "row1", "and", HTuple(RowTargetPosi[0]), Height);
	dilation_circle(SelectedRegions, &RegionDilation, 3.5);
	union1(RegionDilation, &RegionUnionUp);
	count_obj(RegionUnionUp, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d [1]"), Number[0].I());
		*msg = tmpStr;
		ret = 3;
		goto FreeMemory;
	}

	split_skeleton_region(RegionTransDown, &RegionLines, 3);
	select_shape(RegionLines, &SelectedRegions, "row2", "and", 0, HTuple(RowTargetPosi[1]));
	dilation_circle(SelectedRegions, &RegionDilation, 3.5);
	union1(RegionDilation, &RegionUnionDown);
	count_obj(RegionUnionDown, &Number);
	if (0 != (Number != 1))
	{
		tmpStr.Format(_T("Number=%d [1]"), Number[0].I());
		*msg = tmpStr;
		ret = 4;
		goto FreeMemory;
	}

	area_center(RegionUnionUp, &Area, &Row, &Column);
	roiCnPosi[0].x = Column[0].I();
	roiCnPosi[0].y = Row[0].I();
	area_center(RegionUnionDown, &Area, &Row, &Column);
	roiCnPosi[1].x = Column[0].I();
	roiCnPosi[1].y = Row[0].I();

FreeMemory:
	GrayImage.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionClosing.~Hobject();
	RegionFillUp.~Hobject();
	ConnectedRegions.~Hobject();
	DestRegions.~Hobject();
	RegionTransUp.~Hobject();
	RegionTransDown.~Hobject();
	RegionLines.~Hobject();
	SelectedRegions.~Hobject();
	RegionDilation.~Hobject();
	RegionUnionUp.~Hobject();
	RegionUnionDown.~Hobject();

	return 0;
}


int GetSnapChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint* roiCenterPosi, CString* msg)
{
	using namespace Halcon;

	CString tmpStr = _T("");
	int ret = 0;

	// Local iconic variables 
	Hobject  GrayImage, Region, RegionFillUp;
	Hobject  ImageReduced, ImageEmphasize, RegionAll, RegionErosion;
	Hobject  ConnectedRegions, SelectedRegionsAllTarget, SelectedRegionsMarkPoint;
	Hobject  RegionUnion, DestRegions, RegionTrans, RegionDilation;
	Hobject  RegionIntersection, SortedRegions;


	// Local control variables 
	HTuple  Width, Height;
	HTuple  Sigma, AbsoluteHisto, RelativeHisto, MinThresh;
	HTuple  MaxThresh, Length, Number, Area, Row, Column, TmpTargetRow;
	HTuple  TmpTargetColumn, RowV, ColumnV, Area1, RowH, ColumnH;
	HTuple  RoiPosiOffsetRow, RoiPosiOffsetColumn, TargetRow;
	HTuple  TargetColumn, I, Diff;

	gen_image1(&GrayImage, "byte", imgSize.cx, imgSize.cy, (long)pGrayBuff);
	get_image_size(GrayImage, &Width, &Height);

	Sigma = 1;
	do
	{
		gray_histo(GrayImage, GrayImage, &AbsoluteHisto, &RelativeHisto);
		histo_to_thresh(RelativeHisto, Sigma, &MinThresh, &MaxThresh);
		tuple_length(MinThresh, &Length);
		Sigma += 1;
	} while (0 == (Length <= 2));
	threshold(GrayImage, &Region, HTuple(MaxThresh[0]), HTuple(MaxThresh[1]));

	fill_up(Region, &RegionFillUp);
	reduce_domain(GrayImage, RegionFillUp, &ImageReduced);

	emphasize(ImageReduced, &ImageEmphasize, 51, 51, 10);
	threshold(ImageEmphasize, &RegionAll, 0, 80);
	erosion_circle(RegionAll, &RegionErosion, 3.5);

	connection(RegionErosion, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegionsAllTarget, (((HTuple("area").Append("rectangularity")).Append("width")).Append("height")),
		"and", (((HTuple(4000).Append(0.7)).Append(60)).Append(60)), (((HTuple(9000).Append(1)).Append(120)).Append(120)));
	count_obj(SelectedRegionsAllTarget, &Number);
	if (0 != (Number != 13))
	{
		ret = 1;
		CString tmpStr = _T("");
		tmpStr.Format(_T("1 : Number = %d"), Number[0].I());
		*msg = tmpStr;
		goto FreeMemory;
	}

	select_shape(ConnectedRegions, &SelectedRegionsMarkPoint, (((HTuple("area").Append("width")).Append("height")).Append("circularity")),
		"and", (((HTuple(1000).Append(20)).Append(20)).Append(0.8)), (((HTuple(3000).Append(80)).Append(80)).Append(1)));
	count_obj(SelectedRegionsMarkPoint, &Number);
	if (0 != (Number != 4))
	{
		ret = 2;
		CString tmpStr = _T("");
		tmpStr.Format(_T("1 : Number = %d"), Number[0].I());
		*msg = tmpStr;
		goto FreeMemory;
	}

	union1(SelectedRegionsMarkPoint, &RegionUnion);
	area_center(RegionUnion, &Area, &Row, &Column);

	TmpTargetRow = HTuple();
	TmpTargetColumn = HTuple();
	select_region_point(SelectedRegionsAllTarget, &DestRegions, Row, Column);
	area_center(DestRegions, &Area, &Row, &Column);
	TmpTargetRow.Append(Row);
	TmpTargetColumn.Append(Column);

	shape_trans(RegionUnion, &RegionTrans, "convex");
	dilation_rectangle1(RegionTrans, &RegionDilation, 1, Height);
	intersection(SelectedRegionsAllTarget, RegionDilation, &RegionIntersection);
	sort_region(RegionIntersection, &SortedRegions, "first_point", "true", "row");
	area_center(SortedRegions, &Area, &RowV, &ColumnV);

	dilation_rectangle1(RegionTrans, &RegionDilation, Width, 1);
	intersection(SelectedRegionsAllTarget, RegionDilation, &RegionIntersection);
	sort_region(RegionIntersection, &SortedRegions, "first_point", "true", "column");
	area_center(SortedRegions, &Area1, &RowH, &ColumnH);

	TmpTargetRow.Append(HTuple(RowV[0]));
	TmpTargetColumn.Append(HTuple(ColumnV[0]));

	TmpTargetRow.Append(HTuple(RowV[6]));
	TmpTargetColumn.Append(HTuple(ColumnV[6]));

	TmpTargetRow.Append(HTuple(RowH[0]));
	TmpTargetColumn.Append(HTuple(ColumnH[0]));

	TmpTargetRow.Append(HTuple(RowH[6]));
	TmpTargetColumn.Append(HTuple(ColumnH[6]));


	TmpTargetRow.Append(HTuple(RowV[1]));
	TmpTargetColumn.Append(HTuple(ColumnV[1]));

	TmpTargetRow.Append(HTuple(RowV[5]));
	TmpTargetColumn.Append(HTuple(ColumnV[5]));

	TmpTargetRow.Append(HTuple(RowH[1]));
	TmpTargetColumn.Append(HTuple(ColumnH[1]));

	TmpTargetRow.Append(HTuple(RowH[5]));
	TmpTargetColumn.Append(HTuple(ColumnH[5]));


	TmpTargetRow.Append(HTuple(RowV[2]));
	TmpTargetColumn.Append(HTuple(ColumnV[2]));

	TmpTargetRow.Append(HTuple(RowV[4]));
	TmpTargetColumn.Append(HTuple(ColumnV[4]));

	TmpTargetRow.Append(HTuple(RowH[2]));
	TmpTargetColumn.Append(HTuple(ColumnH[2]));

	TmpTargetRow.Append(HTuple(RowH[4]));
	TmpTargetColumn.Append(HTuple(ColumnH[4]));

	RoiPosiOffsetRow = 41;
	RoiPosiOffsetColumn = 5;
	TargetRow = HTuple();
	TargetColumn = HTuple();
	tuple_length(TmpTargetRow, &Length);
	for (I = 0; I <= Length - 1; I += 1)
	{
		TargetRow.Append(HTuple(TmpTargetRow[I]) - RoiPosiOffsetRow);
		TargetColumn.Append(HTuple(TmpTargetColumn[I]) + RoiPosiOffsetColumn);

		TargetRow.Append(HTuple(TmpTargetRow[I]) + RoiPosiOffsetRow);
		TargetColumn.Append(HTuple(TmpTargetColumn[I]) - RoiPosiOffsetColumn);

		TargetRow.Append(HTuple(TmpTargetRow[I]) - RoiPosiOffsetColumn);
		TargetColumn.Append(HTuple(TmpTargetColumn[I]) - RoiPosiOffsetRow);

		TargetRow.Append(HTuple(TmpTargetRow[I]) + RoiPosiOffsetColumn);
		TargetColumn.Append(HTuple(TmpTargetColumn[I]) + RoiPosiOffsetRow);
	}

	tuple_length(TargetRow, &Length);
	for (int i = 0; i <= Length[0].I() - 1; i += 1)
	{
		roiCenterPosi[i].x = TargetColumn[i].I();
		roiCenterPosi[i].y = TargetRow[i].I();
	}

	*msg = _T("Pass");
	ret = 0;

FreeMemory:
	GrayImage.~Hobject();
	Region.~Hobject();
	RegionFillUp.~Hobject();
	ImageReduced.~Hobject();
	ImageEmphasize.~Hobject();
	RegionAll.~Hobject();
	RegionErosion.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegionsAllTarget.~Hobject();
	SelectedRegionsMarkPoint.~Hobject();
	RegionUnion.~Hobject();
	DestRegions.~Hobject();
	RegionTrans.~Hobject();
	RegionDilation.~Hobject();
	RegionIntersection.~Hobject();
	SortedRegions.~Hobject();

	return ret;
}


int GetSnapChartSfrValueROI(USHORT* pRawData, int rawFrm, CSize imgSize, CPoint roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg)
{
	int ret = 0;
	CString tmpStr = _T("");
	if ((roiCnPosi.x == 0) && (roiCnPosi.y == 0))
	{
		ret = 1;
		*sfrValue = 0.0f;
		*msg = _T("Roi posi. failed.");
		return ret;
	}

	int roiWidth = ((roiSize.cx % 2 == 0) ? roiSize.cx : roiSize.cx - 1) + 4;
	int roiHeight = ((roiSize.cy % 2 == 0) ? roiSize.cy : roiSize.cy - 1) + 4;
	int roiStartX = roiCnPosi.x - roiWidth / 2;
	roiStartX = (roiStartX % 2 == 0) ? roiStartX : roiStartX + 1;
	int roiStartY = roiCnPosi.y - roiHeight / 2;
	roiStartY = (roiStartY % 2 == 0) ? roiStartY : roiStartY + 1;

	DWORD blockSize = roiWidth * roiHeight;
	USHORT* blockBuff = new USHORT[blockSize];
	memset(blockBuff, 0, blockSize);
	for (int p = 0; p < roiHeight; p++)
	{
		for (int k = 0; k < roiWidth; k++)
			blockBuff[p*roiWidth + k] = pRawData[(roiStartY + p) * imgSize.cx + (roiStartX + k)] - 16;
	}

	switch ((OUTFORMAT)rawFrm)
	{
	case OUTFORMAT_GRBG:
	case OUTFORMAT_GBRG:
		for (int p = 2; p < roiHeight - 2; p++)
		{
			for (int k = 2; k < roiWidth - 2; k++)
			{
				if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
					;
				else
				{
					int absV = abs(blockBuff[(p - 1) * roiWidth + (k + 0)] - blockBuff[(p + 1) * roiWidth + (k + 0)]);
					int absH = abs(blockBuff[(p + 0) * roiWidth + (k - 1)] - blockBuff[(p + 0) * roiWidth + (k + 1)]);
					blockBuff[p * roiWidth + k] = (absV < absH) ? (blockBuff[(p - 1) * roiWidth + (k + 0)] + blockBuff[(p + 1) * roiWidth + (k + 0)]) / 2 : (blockBuff[(p + 0) * roiWidth + (k - 1)] + blockBuff[(p + 0) * roiWidth + (k + 1)]) / 2;
				}
			}
		}
		break;
	case OUTFORMAT_BGGR:
	case OUTFORMAT_RGGB:
	default:
		for (int p = 2; p < roiHeight - 2; p++)
		{
			for (int k = 2; k < roiWidth - 2; k++)
			{
				if (((p % 2 == 0) && (k % 2 == 0)) || ((p % 2 != 0) && (k % 2 != 0)))
				{
					int absV = abs(blockBuff[(p - 1) * roiWidth + (k + 0)] - blockBuff[(p + 1) * roiWidth + (k + 0)]);
					int absH = abs(blockBuff[(p + 0) * roiWidth + (k - 1)] - blockBuff[(p + 0) * roiWidth + (k + 1)]);
					blockBuff[p * roiWidth + k] = (absV < absH) ? (blockBuff[(p - 1) * roiWidth + (k + 0)] + blockBuff[(p + 1) * roiWidth + (k + 0)]) / 2 : (blockBuff[(p + 0) * roiWidth + (k - 1)] + blockBuff[(p + 0) * roiWidth + (k + 1)]) / 2;
				}
			}
		}
		break;
	}

	DWORD blockFinalSize = (roiWidth - 4) * (roiHeight - 4);
	USHORT* blockFinalBuff = new USHORT[blockFinalSize];
	memset(blockFinalBuff, 0, blockFinalSize);
	DWORD cnt = 0;
	for (int p = 2; p < roiHeight - 2; p++)
	{
		for (int k = 2; k < roiWidth - 2; k++)
			blockFinalBuff[cnt++] = blockBuff[p * roiWidth + k];
	}

	if (blockBuff != NULL)
		delete[] blockBuff;
	blockBuff = NULL;

	int grayLT = blockFinalBuff[0];
	int grayRT = blockFinalBuff[(roiWidth - 4) - 1];
	int grayLB = blockFinalBuff[(roiWidth - 4) * ((roiHeight - 4) - 1)];
	int grayRB = blockFinalBuff[(roiWidth - 4) * (roiHeight - 4) - 1];
	if ((abs(grayLT - grayRB) < 50) || (abs(grayRT - grayLB) < 50))
	{
		tmpStr.Format(_T("ROI image error.%d %d %d %d"), grayLT, grayRT, grayLB, grayRB);
		*msg = tmpStr;
		ret = 2;
		if (blockFinalBuff != NULL)
			delete[] blockFinalBuff;
		blockFinalBuff = NULL;
		return ret;
	}

	CSFRData* sfrData = new CSFRData[256];
	*sfrValue = (float)SFRMatbmpTest(blockFinalBuff, freq, sfrData, roiWidth - 4, roiHeight - 4) + 0.01f;
	if ((*sfrValue < 0.0f) || (*sfrValue > 1.0f))
		*sfrValue = 0.0f;

	if (sfrData != NULL)
		delete[] sfrData;
	sfrData = NULL;

#if 0
	CString blockFinalFileName = _T("");
	CTime t = CTime::GetCurrentTime();
	blockFinalFileName.Format(_T("D:\\Data\\ROI_Block_%.2d%.2d%.2d.raw"), t.GetHour(), t.GetMinute(), t.GetSecond());
	CStdioFile f;
	f.Open(blockFinalFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	f.Write(blockFinalBuff, blockFinalSize * sizeof(USHORT));
	f.Close();
#endif
	if (blockFinalBuff != NULL)
		delete[] blockFinalBuff;
	blockFinalBuff = NULL;

	*msg = _T("Pass");
	ret = 0;
	return ret;
}
//////////	//////////	//////////	//////////	//////////	//////////	//////////	//////////	//////////	//////////