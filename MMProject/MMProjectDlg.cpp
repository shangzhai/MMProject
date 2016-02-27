
// MMProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MMProject.h"
#include "MMProjectDlg.h"
#include "afxdialogex.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/nonfree/features2d.hpp"
#include "opencv/cv.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <io.h>  
#include <direct.h>

using namespace std;
using namespace cv;

#define IMAGE_folder ".."     //folder location
#define IMAGE_LIST_FILE_1 "dataset1"         //the dataset1 for retrieval
#define IMAGE_LIST_FILE_2 "dataset2"      //the dataset2 for detection
#define SEARCH_IMAGE "football.png"     //input information

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double avep[10];
double aver[10];
double aveiou[10];
double aveacc[10];
int countvalue1[10],countvalue2[10];

int countnum1=0,countnum2=0;

int found1=0,found2=0;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMMProjectDlg dialog




CMMProjectDlg::CMMProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMMProjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMMProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, retrievallist);
}

BEGIN_MESSAGE_MAP(CMMProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMMProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CMMProjectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMMProjectDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CMMProjectDlg message handlers

BOOL CMMProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	GetDlgItem(IDC_STATICSEC)->ShowWindow(0);
	retrievallist.AddString("990.jpg");
	retrievallist.AddString("991.jpg");
	retrievallist.AddString("992.jpg");
	retrievallist.AddString("993.jpg");
	retrievallist.AddString("994.jpg");
	retrievallist.AddString("995.jpg");
	retrievallist.AddString("996.jpg");
	retrievallist.AddString("997.jpg");
	retrievallist.AddString("998.jpg");
	retrievallist.AddString("999.jpg");
	retrievallist.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMMProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMMProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMMProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMMProjectDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
CDC *pDC = GetDlgItem(ID)->GetDC();
HDC hDC= pDC->GetSafeHdc();
CRect rect;
GetDlgItem(ID)->GetClientRect(&rect);
CvvImage cimg;
cimg.CopyOf( img ); 
cimg.DrawToHDC( hDC, &rect ); 
ReleaseDC( pDC );
}

void CMMProjectDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
	imageretrieval();
	else
		objectdetection();
}

int  removeDir(char*  dirPath)  //delete specified directory
{  
  
    struct _finddata_t fb;   
    char  path[250];            
    long    handle;  
    int  resultone;  
    int   noFile;            
      
    noFile = 0;  
    handle = 0;  
  
      

    strcpy(path,dirPath);  
    strcat (path,"/*");  
  
    handle = _findfirst(path,&fb);  

    if (handle != 0)  
    {  

        while (0 == _findnext(handle,&fb))  
        {  

            noFile = strcmp(fb.name,"..");  
              
            if (0 != noFile)  
            {  

                memset(path,0,sizeof(path));  
                strcpy(path,dirPath);  
                strcat(path,"/");  
                strcat (path,fb.name);  
 
                if (fb.attrib == 16)  
                {  
                     removeDir(path);     
                }  
 
                else  
                {  
                    remove(path);  
                }  
            }     
        }  
  
        _findclose(handle);  
    }  
        
        resultone = _rmdir(dirPath);  
        return  resultone;  
}  

double compareImgs(CvHistogram *Histogram1, CvHistogram *Histogram2)     
{
	
   
	return cvCompareHist(Histogram1,Histogram2,CV_COMP_BHATTACHARYYA);
}

int CMMProjectDlg::imageretrieval(){

	



	int counttemp=retrievallist.GetCurSel();
	CString searchimg;
	retrievallist.GetLBText(counttemp,searchimg);



	if(countnum1==0)                               //exclude the serial number that has already been entered, only add new entered serial number
	{
		countvalue1[countnum1]=counttemp;
		countnum1++;
	
	}
	else{
		found1=0;
	for(int i=0;i<countnum1;i++)
		if(countvalue1[i]==counttemp)
			{found1=1;
	         break;}
	if(found1==0)
	{countvalue1[countnum1]=counttemp;
	 countnum1++;
	}
	
	}
	
	
	
    
	CEdit* etx=(CEdit*)GetDlgItem(IDC_EDIT1);
	etx->SetWindowTextA("");
	const int filename_len=900;
	char tempname[filename_len]; 

	const int db_size=1000;
	int db_id = 0;

	const int score_size = 10;   //change this to control return top n images
	double minscore[score_size] = {DBL_MAX};
	int minFilename[score_size];

	char minimg_name[filename_len];
	Mat min_img;

	sprintf_s(tempname,filename_len,"%s\\%s\\%s",IMAGE_folder,IMAGE_LIST_FILE_1,searchimg);
	IplImage *image1=cvLoadImage(tempname,0);
	IplImage *image5=cvLoadImage(tempname,1); // read input image
	if( NULL==image1 )                        //check whether the input image exists
	{   etx->SetSel(-1,-1);
		etx->ReplaceSel("Cannot find the input image!\r\n");
		
		
		return -1; 
	}
	
	DrawPicToHDC(image5,IDC_STATIC);              //show input image
	 
	
	IplImage *image3;
	                                            //Necessary information to create Histogram
	int HistogramBins = 256;                    //Bin numbers
    float range[]={0,255};                      //Bin range
    float *HistogramRange[]={range};


    CvHistogram *Histogram1=cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRange);  //create two Histograms for the compared two images
    CvHistogram *Histogram2=cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRange); 
    image3=cvCreateImage(cvSize(500,500),image1->depth,image1->nChannels);        //change the size of image1 and save the result as image3
	cvResize(image1,image3);
    cvCalcHist(&image3,Histogram1,0,0);                                           //calculate the Histogram for image3, save it into Histogram1
     
  
    cvNormalizeHist(Histogram1,1);                                                //Normalize the Histogram1
	cvReleaseImage(&image1);                                                      //Release the memory of image1, image3
	cvReleaseImage(&image3);
     IplImage *image2,*image4;
	//Read Database
	for(db_id;db_id<db_size;db_id++){
		sprintf_s(tempname,filename_len,"%s\\%s\\%d.jpg",IMAGE_folder,IMAGE_LIST_FILE_1,db_id);
		
		image2=cvLoadImage(tempname,0);    //read the compared image
		if( NULL==image2 )
		{   CString err;
		    err.Format("Cannot find the database image number %d!\r\n",db_id+1);
			etx->SetSel(-1,-1);
		    etx->ReplaceSel(err);
			
			return -1; 
		}
		image4=cvCreateImage(cvSize(500,500),image2->depth,image2->nChannels); 
		cvResize(image2,image4);
		cvCalcHist(&image4,Histogram2,0,0);
		cvNormalizeHist(Histogram2,1);
		cvReleaseImage(&image2);
		cvReleaseImage(&image4);
		

		// Apply the Bhattacharyya distance comparison method
		double tempScore = compareImgs(Histogram1, Histogram2);
		
		//store the top k min score ascending
		for(int k=0; k<score_size; k++){
			if(tempScore < minscore[k]){
				for(int k1 = score_size-1; k1>k; k1--){
					minscore[k1] = minscore[k1-1];
					minFilename[k1] = minFilename[k1-1];
				}
				minscore[k] = tempScore;
				minFilename[k] = db_id;
				break;
			}
		}
	}

	//create the folder for saving the retrieval results if it does not exist, or empty it if it already exists
	char dirpath[filename_len];
	sprintf_s(dirpath,filename_len,"%s\\search_results",IMAGE_folder);
	if(_access(dirpath,0)==-1)
		_mkdir(dirpath);
	else
	{removeDir(dirpath);
	_mkdir(dirpath);}

	//read the top k max score image and write them to the a designated folder
	for(int k=0; k<score_size; k++){
		sprintf_s(minimg_name,filename_len,"%s\\%s\\%d.jpg",IMAGE_folder,IMAGE_LIST_FILE_1,minFilename[k]);
		min_img = imread(minimg_name); 
		CString str;
		
		str.Format("The most similar image %d is %d.jpg, the Bhattacharyya distance is %f\r\n",k+1,minFilename[k], minscore[k]);
		
		etx->SetSel(-1,-1);
		etx->ReplaceSel(str);
		sprintf_s(tempname,filename_len,"%s\\search_results\\%d.jpg",IMAGE_folder,minFilename[k]);
		imwrite(tempname, min_img);
		
	}

	//output your precesion and recall (the ground truth are from 990 to 999)
	int count = 0;
	for(int k=0; k<score_size; k++){
		if(minFilename[k]>=990 && minFilename[k]<=999){
			count ++;
		}
	}
	double precision = (double)count/score_size;
	double recall = (double)count/10;

	if(found1==0){                                         //calculate the average of the precision and recall got so far
	avep[countnum1-1]=precision;
	aver[countnum1-1]=recall;}
	double ap=0,ar=0;
	for(int i=0;i<countnum1;i++)
	{ap=ap+avep[i];
	ar=ar+aver[i];
	
	
	}
	ap=ap/countnum1;
	ar=ar/countnum1;

	CString strf;
	strf.Format("The precision and the recall for %s are %.2f and %.2f\r\n",searchimg,precision,recall);
	etx->SetSel(-1,-1);
	etx->ReplaceSel(strf);
	strf.Format("The average precision and recall so far are %.2f and %.2f\r\n",ap,ar);
	etx->SetSel(-1,-1);
	etx->ReplaceSel(strf);
	





	
	

    return 0;


}

int CMMProjectDlg::objectdetection(){
	
	int detectimg=retrievallist.GetCurSel()+1;
	

	if(countnum2==0)                                   //exclude the serial number entered before, only add the new entered serial number
	{
		countvalue2[countnum2]=detectimg;
		countnum2++;
	
	}
	else{
		found2=0;
	for(int i=0;i<countnum2;i++)
		if(countvalue2[i]==detectimg)
			{found2=1;
	         break;}
	if(found2==0)
	{countvalue2[countnum2]=detectimg;
	 countnum2++;
	}
	
	}


	CEdit* etx=(CEdit*)GetDlgItem(IDC_EDIT1);
	etx->SetWindowTextA("");
	const int filename_len=900;
	char tempname[filename_len]; 

	const int score_size=10;  //top n match regions
	double score[score_size] = {DBL_MAX};
	int start_x[score_size], start_y[score_size], end_x[score_size], end_y[score_size];  //store the start location and end location of the detection region (bounding box)
	
	sprintf_s(tempname,filename_len,"%s\\%s",IMAGE_folder,SEARCH_IMAGE);
	IplImage *image1=cvLoadImage(tempname,0);  // read input image
	if( NULL==image1 )
	{   etx->SetSel(-1,-1);
	    etx->ReplaceSel("Cannot find the input image!\r\n");
		
		return -1; 
	}
	DrawPicToHDC(image1,IDC_STATICSEC);    //show the input image
	 
	                                      //Necessary information to create Histogram
	int HistogramBins = 256;              //Bin number
    float range[]={0,255};                //Bin range
    float *HistogramRange[]={range};


    CvHistogram *Histogram1=cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRange);  //create two Histograms
    CvHistogram *Histogram2=cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRange); 

	cvCalcHist(&image1,Histogram1,0,0);               //calculate the Histogram for image1, save it into Histogram1
     
  
    cvNormalizeHist(Histogram1,1);                    //Normalize the Histogram1
	cvReleaseImage(&image1);                          //Release the memory of image1
  
	
	
	//Read detection IMAGE
	sprintf_s(tempname,filename_len,"%s\\%s\\%d.jpg",IMAGE_folder,IMAGE_LIST_FILE_2,detectimg);
	
	IplImage *image2=cvLoadImage(tempname,0);// read besearched image in grayscale
	IplImage *image5=cvLoadImage(tempname,1);// read besearched image again for showing it in color mode
	if( NULL==image2 )                       //check weather the besearched exists
	{   etx->SetSel(-1,-1);
	    etx->ReplaceSel("Cannot find the detection image number!\r\n");
		
		return -1; 
	}
	


	//search the image by bouding box from diffrent scale, location, length-width ratio
	int w = image2->width, h = image2->height;
	//the starting scale in the search, you can change it to a smaller or larger scale
	int scale_w = 20, scale_h = 20;		
	//the ending scale in the search, you can change it to a smaller or larger scale
	int max_scale_w = 150, max_scale_h = 150; 
	//you can change the search step of scale and location in your code, 
	//which will influce both the perforance and speed, you may need a tradeoff
	int scale_step = 5, location_step = 5;      

	//we assume the scale_w should be equals to scale_h in the round ball detection, 
	//thus length-width ratio is always 1 in this algorithmn.
	//For other object, you may need to try different length-width ratio  
	for(scale_w; scale_w<max_scale_w; scale_w+=scale_step, scale_h+=scale_step){   
		int max_x = w - scale_w, max_y = h - scale_h;
		for (int x = 0; x < max_x; x+=location_step) 
			for (int y = 0; y < max_y; y+=location_step)
		{
			//capture the image region in the searching bounding box
			//Mat db_region_img (db_gray_img, Rect(x, y, scale_w, scale_h)); 
			cvSetImageROI(image2, cvRect(x, y, scale_w, scale_h));
			IplImage *image3 = cvCreateImage(cvGetSize(image2),image2->depth,image2->nChannels);
			cvCopy(image2, image3, NULL);
			cvResetImageROI(image2);
			cvCalcHist(&image3,Histogram2,0,0);
		    cvNormalizeHist(Histogram2,1);
		    cvReleaseImage(&image3);
			// Apply the Bhattacharyya distance comparison method
			double tempScore = compareImgs(Histogram1, Histogram2);     
				
			//store the top k(k=score_size) match bounding box and score
			for(int k=0; k<score_size; k++){
				if(tempScore < score[k]){
					for(int k1 = score_size-1; k1>k; k1--){
						score[k1] = score[k1-1];
						start_x[k1] = start_x[k1-1];
						start_y[k1] = start_y[k1-1];
						end_x[k1] = end_x[k1-1];
						end_y[k1] = end_y[k1-1];
					}
					score[k] = tempScore;
					start_x[k] = x;
					start_y[k] = y;
					end_x[k] = x+scale_w;
					end_y[k] = y+scale_h;
					break;
				}
			}
		}
	}



	// draw the best match[top k (k=score_size)] rectangele
	for(int k=0; k<score_size; k++){
		CvPoint start = cvPoint(start_x[k],start_y[k]); 
		CvPoint end = cvPoint(end_x[k],end_y[k]); 
		cvRectangle(image5,start,end,cvScalar(255,0,0));
		  
	}

	//you should keep this evalation code unchanged: 
	//compare your detection boulding box with ground truth bouding box by IoU 
	//first we define the location of ground truth bouding box
	const int gt_start_x[10] = {266,220,200,238,350,26,204,128,33,380};
	const int gt_start_y[10] = {146,248,83,120,80,10,347,258,196,207};
	const int gt_end_x[10] = {353,380,324,314,391,78,248,156,75,404};
	const int gt_end_y[10] = {233,398,207,196,121,62,391,288,238,231};
	//draw ground truth bouding box
	CvPoint start = cvPoint(gt_start_x[detectimg-1],gt_start_y[detectimg-1]); 
	CvPoint end = cvPoint(gt_end_x[detectimg-1],gt_end_y[detectimg-1]); 
	cvRectangle(image5,start,end,cvScalar(0,0,255)); 
	int gt_area = (gt_end_x[detectimg-1] - gt_start_x[detectimg-1]) * (gt_end_y[detectimg-1] - gt_start_y[detectimg-1]);
	//calculate top 10 IoU, and print the best one
	double best_IoU = 0;
	double accuracy=0;
	for(int k=0; k<score_size; k++){
		int intersect_start_x = start_x[k]>gt_start_x[detectimg-1]?start_x[k]:gt_start_x[detectimg-1];
		int intersect_start_y = start_y[k]>gt_start_y[detectimg-1]?start_y[k]:gt_start_y[detectimg-1];
		int intersect_end_x = end_x[k]<gt_end_x[detectimg-1]?end_x[k]:gt_end_x[detectimg-1];
		int intersect_end_y = end_y[k]<gt_end_y[detectimg-1]?end_y[k]:gt_end_y[detectimg-1];

		int your_area = (end_x[k] - start_x[k]) * (end_y[k] - start_y[k]);
		int intersect_area = 0;
		if(intersect_end_x > intersect_start_x && intersect_end_y > intersect_start_y){
			intersect_area = (intersect_end_x - intersect_start_x) * (intersect_end_y - intersect_start_y);
		}
		int union_area = gt_area + your_area - intersect_area;
		double IoU = (double)intersect_area/union_area;
		if(IoU > best_IoU){
			best_IoU = IoU;
		}

	}
	if(best_IoU>0.1)
		accuracy=1;
	if(found2==0)                                 //store the best IoU and accuracy only if it is generated from a new input image, not from the previous images tried before
	{aveiou[countnum2-1]=best_IoU;
	 aveacc[countnum2-1]=accuracy;}
	 double iou=0,acc=0;                               //calculate the average of best IoU and accuracy generated so far
	 for(int i=0;i<countnum2;i++)
	 {iou=iou+aveiou[i];
	  acc=acc+aveacc[i];}
	 iou=iou/countnum2;
	 acc=acc/countnum2;

	 CString str;
	 str.Format( "The best IoU in the top 10 detection of this image is %f\r\n", best_IoU);
	 etx->SetSel(-1,-1);
	 etx->ReplaceSel(str);
	 str.Format( "The average best IoU so far is %f\r\n", iou);
	 etx->SetSel(-1,-1);
	 etx->ReplaceSel(str);
	 if(accuracy==1)
	     str="This image is a correct detection\r\n";
	 else
		 str="This image is not a correct detection\r\n";
	 etx->SetSel(-1,-1);
	 etx->ReplaceSel(str);
	 str.Format( "The accuracy so far is %f\r\n", acc);
	 etx->SetSel(-1,-1);
	 etx->ReplaceSel(str);

	//show and store the detection reuslts
	DrawPicToHDC(image5,IDC_STATIC);
	char dirpath[filename_len];
	sprintf_s(dirpath,filename_len,"%s\\detection_results",IMAGE_folder);    //create the folder for saving image detection results if it does not exist
	if(_access(dirpath,0)==-1)
		_mkdir(dirpath);
	sprintf_s(tempname,filename_len,"%s\\detection_results\\%d.jpg",IMAGE_folder,detectimg);
	cvSaveImage(tempname,image5);
	cvReleaseImage(&image2);                                                //release the memory of image2 and image5
	cvReleaseImage(&image5);

   

    return 0;
}


void CMMProjectDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATICSEC)->ShowWindow(0);
	retrievallist.ResetContent();
	retrievallist.AddString("990.jpg");
	retrievallist.AddString("991.jpg");
	retrievallist.AddString("992.jpg");
	retrievallist.AddString("993.jpg");
	retrievallist.AddString("994.jpg");
	retrievallist.AddString("995.jpg");
	retrievallist.AddString("996.jpg");
	retrievallist.AddString("997.jpg");
	retrievallist.AddString("998.jpg");
	retrievallist.AddString("999.jpg");
	retrievallist.SetCurSel(0);
}


void CMMProjectDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATICSEC)->ShowWindow(1);
	retrievallist.ResetContent();
	retrievallist.AddString("1.jpg");
	retrievallist.AddString("2.jpg");
	retrievallist.AddString("3.jpg");
	retrievallist.AddString("4.jpg");
	retrievallist.AddString("5.jpg");
	retrievallist.AddString("6.jpg");
	retrievallist.AddString("7.jpg");
	retrievallist.AddString("8.jpg");
	retrievallist.AddString("9.jpg");
	retrievallist.AddString("10.jpg");
	retrievallist.SetCurSel(0);
}
