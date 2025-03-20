N=10;
LP=10;
error=-1;
flag=-1;
close all;
[filename,pathname]=uigetfile('*.xls','打开文件');
data=xlsread(strcat(pathname,filename));
   %读取象素点数据
   [row col] = size(data);
   %4 Targets 
   %div4= row/4;
   
   %10 Targets 
   divN= row/N;
%数据分组
Target1=data(1:N:row,:);
Target2=data(2:N:row,:);
Target3=data(3:N:row,:);
Target4=data(4:N:row,:);
Target5=data(5:N:row,:);
Target6=data(6:N:row,:);
Target7=data(7:N:row,:);
Target8=data(8:N:row,:);
Target9=data(9:N:row,:);
Target10=data(10:N:row,:);
   %====================在XYZ坐标系里绘制10个点=================================
   figure(1);
   hold on;
   title('在XYZ坐标系里绘制10个点坐标');
   plot3(Target1(:,3),Target1(:,4),Target1(:,5),'r.');
   plot3(Target2(:,3),Target2(:,4),Target2(:,5),'r.');
   
   plot3(Target3(:,3),Target3(:,4),Target3(:,5),'g.');
   plot3(Target4(:,3),Target4(:,4),Target4(:,5),'g.');
   
   plot3(Target5(:,3),Target5(:,4),Target5(:,5),'b.');
   plot3(Target6(:,3),Target6(:,4),Target6(:,5),'b.');
   
   plot3(Target7(:,3),Target7(:,4),Target7(:,5),'y.');
   plot3(Target8(:,3),Target8(:,4),Target8(:,5),'y.');
   plot3(Target9(:,3),Target9(:,4),Target9(:,5),'y.');
   plot3(Target10(:,3),Target10(:,4),Target10(:,5),'y.');
   legend('T1','T2','T3','T4','T5','T6','T7','T8','T9','T10');
  
   %plot3(T(:,1),T(:,2),T(:,3),'.');
   grid on;
   axis equal;
   
   view(3);
   path1_1=strcat(pathname,'\在XYZ坐标系里绘制10个点坐标.jpg');
   path1_2=strcat(pathname,'\在XYZ坐标系里绘制10个点坐标.fig');

   saveas(1,path1_1);
   saveas(1,path1_2);
   %====================在XYZ坐标系里绘制8个点并连线=================================
   figure(2);hold on;
   title('在XYZ坐标系里绘制10个点坐标并连线');
   plot3(Target1(:,3),Target1(:,4),Target1(:,5),'r.');
   plot3(Target2(:,3),Target2(:,4),Target2(:,5),'r.');
   
   plot3(Target3(:,3),Target3(:,4),Target3(:,5),'g.');
   plot3(Target4(:,3),Target4(:,4),Target4(:,5),'g.');
   
   plot3(Target5(:,3),Target5(:,4),Target5(:,5),'b.');
   plot3(Target6(:,3),Target6(:,4),Target6(:,5),'b.');
   
   plot3(Target7(:,3),Target7(:,4),Target7(:,5),'y.');
   plot3(Target8(:,3),Target8(:,4),Target8(:,5),'y.');
   
   plot3(Target9(:,3),Target9(:,4),Target9(:,5),'m.');
   plot3(Target10(:,3),Target10(:,4),Target10(:,5),'m.');   
   legend('T1','T2','T3','T4','T5','T6','T7','T8','T9','T10');
   
   line([Target1(1:LP:divN,3) Target2(1:LP:divN,3)]',[Target1(1:LP:divN,4) Target2(1:LP:divN,4)]',[Target1(1:LP:divN,5) Target2(1:LP:divN,5)]');
   line([Target3(1:LP:divN,3) Target4(1:LP:divN,3)]',[Target3(1:LP:divN,4) Target4(1:LP:divN,4)]',[Target3(1:LP:divN,5) Target4(1:LP:divN,5)]');
   line([Target5(1:LP:divN,3) Target6(1:LP:divN,3)]',[Target5(1:LP:divN,4) Target6(1:LP:divN,4)]',[Target5(1:LP:divN,5) Target6(1:LP:divN,5)]');
   line([Target7(1:LP:divN,3) Target8(1:LP:divN,3)]',[Target7(1:LP:divN,4) Target8(1:LP:divN,4)]',[Target7(1:LP:divN,5) Target8(1:LP:divN,5)]');
   line([Target9(1:LP:divN,3) Target10(1:LP:divN,3)]',[Target9(1:LP:divN,4) Target10(1:LP:divN,4)]',[Target9(1:LP:divN,5) Target10(1:LP:divN,5)]');
  
   grid on;
   axis equal;
   
   view(3);
   path2_1=strcat(pathname,'\在XYZ坐标系里绘制10个点坐标并连线.jpg');
   path2_2=strcat(pathname,'\在XYZ坐标系里绘制10个点坐标并连线.fig');

   saveas(2,path2_1);
   saveas(2,path2_2);

   
   %==================计算角度变化初始角YZ==================================
   IA12 = atan2((Target1(1,5)-Target2(1,5)),(Target1(1,4)-Target2(1,4)))/pi*180;
   IA34 = atan2((Target3(1,5)-Target4(1,5)),(Target3(1,4)-Target4(1,4)))/pi*180;
   IA56 = atan2((Target5(1,5)-Target6(1,5)),(Target5(1,4)-Target6(1,4)))/pi*180;
   IA78 = atan2((Target7(1,5)-Target8(1,5)),(Target7(1,4)-Target8(1,4)))/pi*180;
  
       if IA12>90
          IA12=IA12-180;
       elseif IA12<-90
          IA12=IA12+180;
       end
       
       if IA34>90
          IA34=IA34-180;
       elseif IA34<-90
          IA34=IA34+180;
       end
       
       if IA56>90
          IA56=IA56-180;
       elseif IA56<-90
          IA56=IA56+180;
       end
       
       if IA78>90
          IA78=IA78-180;
       elseif IA78<-90
          IA78=IA78+180;
       end
  
   
   %=================计算绝对角度变化量YZ========================================
   Lyz12 = atan2((Target1(:,5)-Target2(:,5)),(Target1(:,4)-Target2(:,4)))/pi*180 - IA12;
   Lyz34 = atan2((Target3(:,5)-Target4(:,5)),(Target3(:,4)-Target4(:,4)))/pi*180 - IA34;
   Lyz56 = atan2((Target5(:,5)-Target6(:,5)),(Target5(:,4)-Target6(:,4)))/pi*180 - IA56;
   Lyz78 = atan2((Target7(:,5)-Target8(:,5)),(Target7(:,4)-Target8(:,4)))/pi*180 - IA78;
   
   for i=1:divN
       if Lyz12(i)>90
          Lyz12(i)=Lyz12(i)-180;
       elseif Lyz12(i)<-90
          Lyz12(i)=Lyz12(i)+180;
       end
       
       if Lyz34(i)>90
          Lyz34(i)=Lyz34(i)-180;
       elseif Lyz34(i)<-90
          Lyz34(i)=Lyz34(i)+180;
       end
       
       
       if Lyz56(i)>90
          Lyz56(i)=Lyz56(i)-180;
       elseif Lyz56(i)<-90
          Lyz56(i)=Lyz56(i)+180;
       end
       
       if Lyz78(i)>90
          Lyz78(i)=Lyz78(i)-180;
       elseif Lyz78(i)<-90
          Lyz78(i)=Lyz78(i)+180;
       end
   end
   
     

     %=================计算相对角度变化量YZ========================================
   Lyz12_34 =Lyz12-Lyz34;
   Lyz34_56 =Lyz34-Lyz56;
   Lyz56_78 =Lyz56-Lyz78; 
for i=1:divN
    if Lyz12_34(i)>90
      Lyz12_34(i)=Lyz12_34(i)-180;
    elseif Lyz12_34(i)<-90
      Lyz12_34(i)=Lyz12_34(i)+180;
    end


   
    if Lyz34_56(i)>90
       Lyz34_56(i)=Lyz34_56(i)-180;
    elseif Lyz34_56(i)<-90
       Lyz34_56(i)=Lyz34_56(i)+180;
    end

  
    if Lyz56_78(i)>90
       Lyz56_78(i)=Lyz56_78(i)-180;
    elseif Lyz56_78(i)<-90
       Lyz56_78(i)=Lyz56_78(i)+180;
    end
end

   Lyz12_78 =abs(Lyz12_34)+abs(Lyz34_56)+abs(Lyz56_78);

    %=================计算相对1112号角度变化量YZ========================================
    LyzA_p=zeros(divN,1);
    LyzB_p=zeros(divN,1);
    LyzC_p=zeros(divN,1);
    for i=2:divN
       if -0.005<Lyz12_78(i)<0.005
           LyzA_p(i)=0;
           LyzB_p(i)=0;
           LyzC_p(i)=0;
       else
           LyzA_p(i) =abs((Lyz12_34(i)/Lyz12_78(i)))*100; 
           LyzB_p(i) =abs((Lyz34_56(i)/Lyz12_78(i)))*100;
           LyzC_p(i) =abs((Lyz56_78(i)/Lyz12_78(i)))*100;  
       end
    end
   %===================绘图===============================================
   
   %==================计算角度变化初始角XY==================================
   IB12 = atan2((Target1(1,4)-Target2(1,4)),(Target1(1,3)-Target2(1,3)))/pi*180;
   IB34 = atan2((Target3(1,4)-Target4(1,4)),(Target3(1,3)-Target4(1,3)))/pi*180;
   IB56 = atan2((Target5(1,4)-Target6(1,4)),(Target5(1,3)-Target6(1,3)))/pi*180;
   IB78 = atan2((Target7(1,4)-Target8(1,4)),(Target7(1,3)-Target8(1,3)))/pi*180;


       if IB12>90
          IB12=IB12-180;
       elseif IB12<-90
          IB12=IB12+180;
       end
       
       if IB34>90
          IB34=IB34-180;
       elseif IB34<-90
          IB34=IB34+180;
       end
       
       if IB56>90
          IB56=IB56-180;
       elseif IB56<-90
          IB56=IB56+180;
       end
       
       if IB78>90
          IB78=IB78-180;
       elseif IB78<-90
          IB78=IB78+180;
       end
       
  


   
   %=================计算绝对角度变化量XY========================================
   Lxy12 = atan2((Target1(:,4)-Target2(:,4)),(Target1(:,3)-Target2(:,3)))/pi*180 - IB12;
   Lxy34 = atan2((Target3(:,4)-Target4(:,4)),(Target3(:,3)-Target4(:,3)))/pi*180 - IB34;
   Lxy56 = atan2((Target5(:,4)-Target6(:,4)),(Target5(:,3)-Target6(:,3)))/pi*180 - IB56;
   Lxy78 = atan2((Target7(:,4)-Target8(:,4)),(Target7(:,3)-Target8(:,3)))/pi*180 - IB78;
   for i=1:divN
       if Lxy12(i)>90
          Lxy12(i)=Lxy12(i)-180;
       elseif Lxy12(i)<-90
          Lxy12(i)=Lxy12(i)+180;
       end
       
       if Lxy34(i)>90
          Lxy34(i)=Lxy34(i)-180;
       elseif Lxy34(i)<-90
          Lxy34(i)=Lxy34(i)+180;
       end
      
       if Lxy56(i)>90
          Lxy56(i)=Lxy56(i)-180;
       elseif Lxy56(i)<-90
          Lxy56(i)=Lxy56(i)+180;
       end
       
       if Lxy78(i)>90
          Lxy78(i)=Lxy78(i)-180;
       elseif Lxy78(i)<-90
          Lxy78(i)=Lxy78(i)+180;
       end
   end
     %=================计算相对角度变化量XY========================================
   Lxy12_34 =Lxy12-Lxy34;
   Lxy34_56 =Lxy34-Lxy56;
   Lxy56_78 =Lxy56-Lxy78; 

for i=1:divN
   if Lxy12_34(i)>90
      Lxy12_34(i)=Lxy12_34(i)-180;
    elseif Lxy12_34(i)<-90
      Lxy12_34(i)=Lxy12_34(i)+180;
    end

    if Lxy34_56(i)>90
       Lxy34_56(i)=Lxy34_56(i)-180;
    elseif Lxy34_56(i)<-90
       Lxy34_56(i)=Lxy34_56(i)+180;
    end

    if Lxy56_78(i)>90
       Lxy56_78(i)=Lxy56_78(i)-180;
    elseif Lxy56_78(i)<-90
       Lxy56_78(i)=Lxy56_78(i)+180;
    end
end

   Lxy12_78 =abs(Lxy12_34)+abs(Lxy34_56)+abs(Lxy56_78);
    %=================计算相对1112号角度变化量XY=====================================
    LxyA_p=zeros(divN,1);
    LxyB_p=zeros(divN,1);
    LxyC_p=zeros(divN,1);
    for i=2:divN
       if -0.005<Lxy12_78(i)<0.005
           LxyA_p(i)=0;
           LxyB_p(i)=0;
           LxyC_p(i)=0;
       else
           LxyA_p(i) =abs((Lxy12_34(i)/Lxy12_78(i))*100); 
           LxyB_p(i) =abs((Lxy34_56(i)/Lxy12_78(i))*100);
           LxyC_p(i) =abs((Lxy56_78(i)/Lxy12_78(i))*100);
       end
    end
    
   %==================计算角度变化初始角XZ==================================
   IC12 = atan2((Target1(1,5)-Target2(1,5)),(Target1(1,3)-Target2(1,3)))/pi*180;
   IC34 = atan2((Target3(1,5)-Target4(1,5)),(Target3(1,3)-Target4(1,3)))/pi*180;
   IC56 = atan2((Target5(1,5)-Target6(1,5)),(Target5(1,3)-Target6(1,3)))/pi*180;
   IC78 = atan2((Target7(1,5)-Target8(1,5)),(Target7(1,3)-Target8(1,3)))/pi*180;

       if IC12>90
          IC12=IC12-180;
       elseif IC12<-90
          IC12=IC12+180;
       end
       
       if IC34>90
          IC34=IC34-180;
       elseif IC34<-90
          IC34=IC34+180;
       end
       
       if IC56>90
          IC56=IC56-180;
       elseif IC56<-90
          IC56=IC56+180;
       end
       
       if IC78>90
          IC78=IC78-180;
       elseif IC78<-90
          IC78=IC78+180;
       end
   

   
   %=================计算绝对角度变化量XZ========================================
   Lxz12 = atan2((Target1(:,5)-Target2(:,5)),(Target1(:,3)-Target2(:,3)))/pi*180 - IC12;
   Lxz34 = atan2((Target3(:,5)-Target4(:,5)),(Target3(:,3)-Target4(:,3)))/pi*180 - IC34;
   Lxz56 = atan2((Target5(:,5)-Target6(:,5)),(Target5(:,3)-Target6(:,3)))/pi*180 - IC56;
   Lxz78 = atan2((Target7(:,5)-Target8(:,5)),(Target7(:,3)-Target8(:,3)))/pi*180 - IC78;
   for i=1:divN
       if Lxz12(i)>90
          Lxz12(i)=Lxz12(i)-180;
       elseif Lxz12(i)<-90
          Lxz12(i)=Lxz12(i)+180;
       end
       
       if Lxz34(i)>90
          Lxz34(i)=Lxz34(i)-180;
       elseif Lxz34(i)<-90
          Lxz34(i)=Lxz34(i)+180;
       end
       
       if Lxz56(i)>90
          Lxz56(i)=Lxz56(i)-180;
       elseif Lxz56(i)<-90
          Lxz56(i)=Lxz56(i)+180;
       end
       
       if Lxz78(i)>90
          Lxz78(i)=Lxz78(i)-180;
       elseif Lxz78(i)<-90
          Lxz78(i)=Lxz78(i)+180;
       end
   end
    %=================计算相对角度变化量XZ========================================

   Lxz12_34 =Lxz12-Lxz34;
   Lxz34_56 =Lxz34-Lxz56;
   Lxz56_78 =Lxz56-Lxz78;
for i=1:divN
    if Lxz12_34(i)>90
      Lxz12_34(i)=Lxz12_34(i)-180;
    elseif Lxz12_34(i)<-90
      Lxz12_34(i)=Lxz12_34(i)+180;
    end


  
    if Lxz34_56(i)>90
       Lxz34_56(i)=Lxz34_56(i)-180;
    elseif Lxz34_56(i)<-90
       Lxz34_56(i)=Lxz34_56(i)+180;
    end

    if Lxz56_78(i)>90
       Lxz56_78(i)=Lxz56_78(i)-180;
    elseif Lxz56_78(i)<-90
       Lxz56_78(i)=Lxz56_78(i)+180;
       end
end

   Lxz12_78 =abs(Lxz12_34)+abs(Lxz34_56)+abs(Lxz56_78);

    %=================计算相对1112号角度变化量XZ========================================
    LxzA_p=zeros(divN,1);
    LxzB_p=zeros(divN,1);
    LxzC_p=zeros(divN,1);
    for i=2:divN
       if -0.005<Lxz12_78(i)<0.005
           LxzA_p(i)=0;
           LxzB_p(i)=0;
           LxzC_p(i)=0;
       else
           LxzA_p(i) =abs((Lxz12_34(i)/Lxz12_78(i))*100); 
           LxzB_p(i) =abs((Lxz34_56(i)/Lxz12_78(i))*100);
           LxzC_p(i) =abs((Lxz56_78(i)/Lxz12_78(i))*100);
       end
    end

   %===================绘图===============================================

figure(3);

subplot(3,1,1);hold on;
title('绝对角度变化量');
   plot(Lyz12,'b');
   plot(Lyz34,'r');
   plot(Lyz56,'g');
   plot(Lyz78,'y');
   grid on;
   legend('Lyz12','Lyz34','Lyz56','Lyz78');
   xlabel('绝对角度变化量YZ');
   
subplot(3,1,2);hold on;
   plot(Lxy12,'b');
   plot(Lxy34,'r');
   plot(Lxy56,'g');
   plot(Lxy78,'y');
   grid on;
   legend('Lxy12','Lxy34','Lxy56','Lxy78');
   xlabel('绝对角度变化量XY');
   
subplot(3,1,3);hold on;
   plot(Lxz12,'b');
   plot(Lxz34,'r');
   plot(Lxz56,'g');
   plot(Lxz78,'y');
   grid on;
   legend('Lxz12','Lxz34','Lxz56','Lxz78');
   xlabel('绝对角度变化量XZ');


   
path3_1=strcat(pathname,'\绝对角度变化量.jpg');
path3_2=strcat(pathname,'\绝对角度变化量.fig');
saveas(3,path3_1);
saveas(3,path3_2);

    %===================绘图===============================================
figure(4);

subplot(3,1,1); hold on;
title('相对角度变化量');
   plot(Lyz12_34,'b');
   plot(Lyz34_56,'r');
   plot(Lyz56_78,'g');
   plot(Lyz12_78,'y');
   grid on;
   legend('Lyz12-34','Lyz34-56','Lyz56-78','Lyz12-78');
   xlabel('相对角度变化量YZ');
   
subplot(3,1,2); hold on;
   plot(Lxy12_34,'b');
   plot(Lxy34_56,'r');
   plot(Lxy56_78,'g');
   plot(Lxy12_78,'y');
   grid on;
   legend('Lxy12-34','Lxy34-56','Lxy56-78','Lxy12-78');
   xlabel('相对角度变化量XY');
   
subplot(3,1,3);hold on;
   plot(Lxz12_34,'b');
   plot(Lxz34_56,'r');
   plot(Lxz56_78,'g');
   plot(Lxz12_78,'y');
  % ylim([-90 90]);
   grid on;
   legend('Lxz12-34','Lxz34-56','Lxz56-78','Lxz12-78');
   xlabel('相对角度变化量XZ');
   

path4_1=strcat(pathname,'\相对角度变化量.jpg');
path4_2=strcat(pathname,'\相对角度变化量.fig');
saveas(4,path4_1);
saveas(4,path4_2);

flag=1;
     %===================绘图===============================================
figure(5);
  
subplot(3,1,1);hold on;
title('相对角度变化量占总角度变化量的百分比'); 
   plot(LyzA_p,'b');
   plot(LyzB_p,'r');
   plot(LyzC_p,'g');
   ylim([0 100]);
   grid on;
   legend('LyzA','LyzB','LyzC');
   xlabel('相对角度变化量YZ占总角度变化量的百分比');
   
subplot(3,1,2);hold on;
   plot(LxyA_p,'b');
   plot(LxyB_p,'r');
   plot(LxyC_p,'g');
   ylim([0 100]);
   grid on;
   legend('LxyA','LxyB','LxyC');
   xlabel('相对角度变化量XY占总角度变化量的百分比');
   
subplot(3,1,3);hold on;
   plot(LxzA_p,'b');
   plot(LxzB_p,'r');
   plot(LxzC_p,'g');
   ylim([0 100]);
   grid on;
   legend('LxzA','LxzB','LxzC');
   xlabel('相对角度变化量XZ占总角度变化量的百分比');

   

path5_1=strcat(pathname,'相对角度变化量占总角度变化量的百分比.jpg');
path5_2=strcat(pathname,'相对角度变化量占总角度变化量的百分比.fig');
saveas(5,path5_1);
saveas(5,path5_2);
%saveas(5,'\相对角度变化量占总角度变化量的百分比.jpg');
%saveas(5,'\相对角度变化量占总角度变化量的百分比.fig');
out=[Lxy12_34,Lxy34_56,Lxy56_78,Lxy12_78,Lxy12,Lxy34,Lxy56,Lxy78,LxyA_p,LxyB_p,LxyC_p,Lxz12_34,Lxz34_56,Lxz56_78,Lxz12_78,Lxz12,Lxz34,Lxz56,Lxz78,LxzA_p,LxzB_p,LxzC_p,Lyz12_34,Lyz34_56,Lyz56_78,Lyz12_78,Lyz12,Lyz34,Lyz56,Lyz78,LyzA_p,LyzB_p,LyzC_p];
out(1,:)=0;
error=0;

