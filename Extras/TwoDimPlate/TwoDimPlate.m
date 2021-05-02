clear
close all
clc

%Defining variables
nx = 15;
ny = nx;
t = 500;%Total time
dt = 10;%timestep
nt = t/dt;%no. of timesteps

x = linspace(0,1,nx);
y = linspace(0,1,ny);
[xx,yy] = meshgrid(x,y);      
T = 500*ones(nx,ny,nt);

%Thermal diffusivity of copper
alpha = 113e-6;
dx = 1/(nx-1);
dy = dx;
c = (alpha*dt)/(dx^2);

%Assigning boundary conditions
for k = 1:nt+1
  T(1,:,k)     =    0;
  T(:,1,k)     =    0;
  T(end,end,k) = 1000;
  for i = 1:nx
    T(i,end,k)   = (0*(nx-i+1)+1000*(i-1))/nx;
  end
  for i = 1:ny
    T(end,i,k)   = (0*(ny-i+1)+1000*(i-1))/ny;
  end
  T(1,1,k)     =    0;
  T(1,end,k)   =    0;
  T(end,1,k)   =    0;
  T(end,end,k) = 1000; 
end
k1 = (alpha*dt)/dx^2;
k2 = (alpha*dt)/dy^2;
T_initial = T(:,:,1);%Initial temperature as per boundary condition

%Time Loop
for k = 1:nt
    %Nodal Loops
    for i = 2:nx-1
        for j = 2:ny-1
            T(i,j,k+1) = (T(i,j,k)+(k1*(T(i+1,j,k)+T(i-1,j,k)-(2*T(i,j,k))))+(k2*(T(i,j+1,k)+T(i,j-1,k)-(2*T(i,j,k)))));
        end
    end 
end
T_final = T(:,:,nt);

%Plotting the temperature change
figure(1)
subplot(2,2,1)
surf(xx,yy,T_initial)
view(2)
shading interp
title('Initial Temperature along the plate surface')
colorbar;
subplot(2,2,2)
surf(xx,yy,T_final)
view(2)
shading interp
title('Final Temperature along the plate surface')
colorbar;
subplot(2,1,2)
ct = 1;
for k = 1:nt
    Time = sprintf('Time = %d%s',k*20,'sec');
    T_time = T(:,:,k);
    surf(xx,yy,T_time)
    set(gcf,'units','normalized','outerposition',[0 0 1 1])
    shading interp
    title({'Temperature change with respect to time';Time})
    colorbar;
    M(ct)=getframe(gcf);
    pause(1.0)
    ct=ct+1;
end
movie(M,1,1)
video_file=VideoWriter('ExplicitHE.avi','uncompressed AVI');
open(video_file)
writeVideo(video_file,M)
close(video_file)