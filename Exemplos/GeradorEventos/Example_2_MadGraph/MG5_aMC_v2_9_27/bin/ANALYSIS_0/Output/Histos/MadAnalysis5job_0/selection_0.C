void selection_0()
{

  // ROOT version
  Int_t root_version = gROOT->GetVersionInt();

  // Creating a new TCanvas
  TCanvas* canvas = new TCanvas("canvas_plotflow_tempo1","canvas_plotflow_tempo1",0,0,700,500);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  canvas->SetHighLightColor(2);
  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetBorderSize(3);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(0);
  canvas->SetTickx(1);
  canvas->SetTicky(1);
  canvas->SetLeftMargin(0.14);
  canvas->SetRightMargin(0.05);
  canvas->SetBottomMargin(0.15);
  canvas->SetTopMargin(0.05);

  // Creating a new TH1F
  TH1F* S1_M_0 = new TH1F("S1_M_0","S1_M_0",100,0.0,1000.0);
  // Content
  S1_M_0->SetBinContent(0,0.0); // underflow
  S1_M_0->SetBinContent(1,0.0);
  S1_M_0->SetBinContent(2,0.0);
  S1_M_0->SetBinContent(3,1029433.0367200032);
  S1_M_0->SetBinContent(4,514716.5183600016);
  S1_M_0->SetBinContent(5,234650.20837000184);
  S1_M_0->SetBinContent(6,147182.00525000025);
  S1_M_0->SetBinContent(7,81580.88291000015);
  S1_M_0->SetBinContent(8,144658.90516000096);
  S1_M_0->SetBinContent(9,1500415.0535199898);
  S1_M_0->SetBinContent(10,4500405.1605300065);
  S1_M_0->SetBinContent(11,149705.10533999954);
  S1_M_0->SetBinContent(12,42893.041530000075);
  S1_M_0->SetBinContent(13,19343.92069000003);
  S1_M_0->SetBinContent(14,15138.720540000026);
  S1_M_0->SetBinContent(15,5887.28021000001);
  S1_M_0->SetBinContent(16,5887.28021000001);
  S1_M_0->SetBinContent(17,1682.0800600000027);
  S1_M_0->SetBinContent(18,3364.1601200000055);
  S1_M_0->SetBinContent(19,1682.0800600000027);
  S1_M_0->SetBinContent(20,2523.120090000004);
  S1_M_0->SetBinContent(21,841.0400300000014);
  S1_M_0->SetBinContent(22,1682.0800600000027);
  S1_M_0->SetBinContent(23,0.0);
  S1_M_0->SetBinContent(24,3364.1601200000055);
  S1_M_0->SetBinContent(25,0.0);
  S1_M_0->SetBinContent(26,0.0);
  S1_M_0->SetBinContent(27,841.0400300000014);
  S1_M_0->SetBinContent(28,841.0400300000014);
  S1_M_0->SetBinContent(29,0.0);
  S1_M_0->SetBinContent(30,841.0400300000014);
  S1_M_0->SetBinContent(31,0.0);
  S1_M_0->SetBinContent(32,0.0);
  S1_M_0->SetBinContent(33,0.0);
  S1_M_0->SetBinContent(34,0.0);
  S1_M_0->SetBinContent(35,0.0);
  S1_M_0->SetBinContent(36,0.0);
  S1_M_0->SetBinContent(37,0.0);
  S1_M_0->SetBinContent(38,841.0400300000014);
  S1_M_0->SetBinContent(39,0.0);
  S1_M_0->SetBinContent(40,0.0);
  S1_M_0->SetBinContent(41,0.0);
  S1_M_0->SetBinContent(42,0.0);
  S1_M_0->SetBinContent(43,0.0);
  S1_M_0->SetBinContent(44,0.0);
  S1_M_0->SetBinContent(45,0.0);
  S1_M_0->SetBinContent(46,0.0);
  S1_M_0->SetBinContent(47,0.0);
  S1_M_0->SetBinContent(48,0.0);
  S1_M_0->SetBinContent(49,0.0);
  S1_M_0->SetBinContent(50,0.0);
  S1_M_0->SetBinContent(51,0.0);
  S1_M_0->SetBinContent(52,0.0);
  S1_M_0->SetBinContent(53,0.0);
  S1_M_0->SetBinContent(54,0.0);
  S1_M_0->SetBinContent(55,0.0);
  S1_M_0->SetBinContent(56,0.0);
  S1_M_0->SetBinContent(57,0.0);
  S1_M_0->SetBinContent(58,0.0);
  S1_M_0->SetBinContent(59,0.0);
  S1_M_0->SetBinContent(60,0.0);
  S1_M_0->SetBinContent(61,0.0);
  S1_M_0->SetBinContent(62,0.0);
  S1_M_0->SetBinContent(63,0.0);
  S1_M_0->SetBinContent(64,0.0);
  S1_M_0->SetBinContent(65,0.0);
  S1_M_0->SetBinContent(66,0.0);
  S1_M_0->SetBinContent(67,0.0);
  S1_M_0->SetBinContent(68,0.0);
  S1_M_0->SetBinContent(69,0.0);
  S1_M_0->SetBinContent(70,0.0);
  S1_M_0->SetBinContent(71,0.0);
  S1_M_0->SetBinContent(72,0.0);
  S1_M_0->SetBinContent(73,0.0);
  S1_M_0->SetBinContent(74,0.0);
  S1_M_0->SetBinContent(75,0.0);
  S1_M_0->SetBinContent(76,0.0);
  S1_M_0->SetBinContent(77,0.0);
  S1_M_0->SetBinContent(78,0.0);
  S1_M_0->SetBinContent(79,0.0);
  S1_M_0->SetBinContent(80,0.0);
  S1_M_0->SetBinContent(81,0.0);
  S1_M_0->SetBinContent(82,0.0);
  S1_M_0->SetBinContent(83,0.0);
  S1_M_0->SetBinContent(84,0.0);
  S1_M_0->SetBinContent(85,0.0);
  S1_M_0->SetBinContent(86,0.0);
  S1_M_0->SetBinContent(87,0.0);
  S1_M_0->SetBinContent(88,0.0);
  S1_M_0->SetBinContent(89,0.0);
  S1_M_0->SetBinContent(90,0.0);
  S1_M_0->SetBinContent(91,0.0);
  S1_M_0->SetBinContent(92,0.0);
  S1_M_0->SetBinContent(93,0.0);
  S1_M_0->SetBinContent(94,0.0);
  S1_M_0->SetBinContent(95,0.0);
  S1_M_0->SetBinContent(96,0.0);
  S1_M_0->SetBinContent(97,0.0);
  S1_M_0->SetBinContent(98,0.0);
  S1_M_0->SetBinContent(99,0.0);
  S1_M_0->SetBinContent(100,0.0);
  S1_M_0->SetBinContent(101,0.0); // overflow
  S1_M_0->SetEntries(10000);
  // Style
  S1_M_0->SetLineColor(9);
  S1_M_0->SetLineStyle(1);
  S1_M_0->SetLineWidth(1);
  S1_M_0->SetFillColor(9);
  S1_M_0->SetFillStyle(1001);

  // Creating a new THStack
  THStack* stack = new THStack("mystack_2","mystack");
  stack->Add(S1_M_0);
  stack->Draw("");

  // Y axis
  stack->GetYaxis()->SetLabelSize(0.04);
  stack->GetYaxis()->SetLabelOffset(0.005);
  stack->GetYaxis()->SetTitleSize(0.04);
  stack->GetYaxis()->SetTitleFont(22);
  stack->GetYaxis()->SetTitleOffset(1);
  stack->GetYaxis()->SetTitle("N. of mu+ mu- pairs  ( L_{int} = 10 fb^{-1} )");

  // X axis
  stack->GetXaxis()->SetLabelSize(0.04);
  stack->GetXaxis()->SetLabelOffset(0.005);
  stack->GetXaxis()->SetTitleSize(0.06);
  stack->GetXaxis()->SetTitleFont(22);
  stack->GetXaxis()->SetTitleOffset(1);
  stack->GetXaxis()->SetTitle("M [ mu+ mu- ] (GeV/c^{2}) ");

  // Finalizing the TCanvas
  canvas->SetLogx(0);
  canvas->SetLogy(0);

  // Saving the image
  canvas->SaveAs("../../HTML/MadAnalysis5job_0/selection_0.png");
  canvas->SaveAs("../../PDF/MadAnalysis5job_0/selection_0.png");
  canvas->SaveAs("../../DVI/MadAnalysis5job_0/selection_0.eps");

}
