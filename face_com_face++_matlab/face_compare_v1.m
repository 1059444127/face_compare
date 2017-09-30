function varargout = face_compare_v1(varargin)
% FACE_COMPARE_V1 MATLAB code for face_compare_v1.fig
%      FACE_COMPARE_V1, by itself, creates a new FACE_COMPARE_V1 or raises the existing
%      singleton*.
%
%      H = FACE_COMPARE_V1 returns the handle to a new FACE_COMPARE_V1 or the handle to
%      the existing singleton*.
%
%      FACE_COMPARE_V1('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FACE_COMPARE_V1.M with the given input arguments.
%
%      FACE_COMPARE_V1('Property','Value',...) creates a new FACE_COMPARE_V1 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before face_compare_v1_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to face_compare_v1_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help face_compare_v1

% Last Modified by GUIDE v2.5 05-Aug-2017 21:31:13

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @face_compare_v1_OpeningFcn, ...
                   'gui_OutputFcn',  @face_compare_v1_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end

% End initialization code - DO NOT EDIT


% --- Executes just before face_compare_v1 is made visible.
function face_compare_v1_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to face_compare_v1 (see VARARGIN)

% Choose default command line output for face_compare_v1
handles.output = hObject;
%%%%%%%%%%%%%%%%%%
h = handles.figure1; %��������
newIcon = javax.swing.ImageIcon('./image/icon/camera.png');
figFrame = get(h,'JavaFrame'); %ȡ��Figure��JavaFrame��
figFrame.setFigureIcon(newIcon); %�޸�ͼ��

API_KEY = 'your key';   %�����Լ���key��secret
API_SECRET = 'your secret';

% If you have chosen Amazon as your API sever and 
% changed API_KEY&API_SECRET into yours, 
% pls reform the FACEPP call as following :
% api = facepp(API_KEY, API_SECRET, 'US')
global api;
api= facepp(API_KEY, API_SECRET);

global img img0 pre pre0;
str='.\image\lena.jpg';
pre=str;
pre0=str;
 img=imread(str);
axes(handles.axes1);
imshow(img);
str0=str;
img0=imread(str0);
axes(handles.axes2);
imshow(img0);
set(handles.select1,'userdata',str);
set(handles.select2,'userdata',str0);
% Update handles structure
guidata(hObject, handles);
set(handles.text1,'String',str);
set(handles.text2,'String',str0);
set(handles.text4,'fontsize',12,'String','��ʼ����ע�����Ҫ����������⣬�������������������Աȡ�Ҳ����һ������');
% UIWAIT makes face_compare_v1 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = face_compare_v1_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in select1.
function select1_Callback(hObject, eventdata, handles)
% hObject    handle to select1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global pre img;
default_path='./image';
[filename,pathname]=uigetfile({'*.jpg;*.png;*bmp;*.pgm','All Picture Files (*.jpg, *.png, *.bmp, *.pgm)';'*.*',  'All Files (*.*)'},'ѡ��ͼƬ',default_path);
%�ϳ�ͼƬ·��+�ļ���
str=[pathname filename];
if (sum(size(str))>4)
    axes(handles.axes1) ;
    cla reset;
    pre=str;
    set(handles.text4,'string','�ɹ���ȡͼƬ1');
else 
    set(handles.text4,'string','û��ѡ��ͼƬ�������ϴε�ͼƬ');
end;
set(handles.select1,'userdata',pre);
set(handles.text1,'String',pre);
%��ȡͼƬ
img=imread(pre);%ʹ�õ�һ��axes
% img=imresize(img,[640,480]);
%image_1=double(img);
axes(handles.axes1);
imshow(img);



% --- Executes on button press in select2.
function select2_Callback(hObject, eventdata, handles)
% hObject    handle to select2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global pre0 img0
default_path='./image';
[filename,pathname]=uigetfile({'*.jpg;*.png;*bmp;*.pgm','All Picture Files (*.jpg, *.png, *.bmp, *.pgm)';'*.*',  'All Files (*.*)'},'ѡ��ͼƬ',default_path);
%�ϳ�ͼƬ·��+�ļ���
str0=[pathname filename];
if (sum(size(str0))>4)
    axes(handles.axes2) ;
    cla reset;
    pre0=str0;
    set(handles.text4,'string','�ɹ���ȡͼƬ2');
else 
    set(handles.text4,'string','û��ѡ��ͼƬ�������ϴε�ͼƬ');
end
set(handles.select2,'userdata',pre0);
set(handles.text2,'String',pre0);
%��ȡͼƬ
 img0=imread(pre0);
%image_0=double(img0);
%ʹ�õڶ���axes
axes(handles.axes2);
imshow(img0);


% --- Executes on button press in detect.
function detect_Callback(hObject, eventdata, handles)
% hObject    handle to detect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.text4,'string','');
tic;
global api  img0 img;
str=get(handles.select1,'userdata');
str0=get(handles.select2,'userdata');
global rst rst0
rst = detect_file(api, str, 'all');
rst0=detect_file(api,str0,'all');
img_width = rst{1}.img_width;
img_height = rst{1}.img_height;
img_width0 = rst0{1}.img_width;
img_height0 = rst0{1}.img_height;
face = rst{1}.face;
face0=rst0{1}.face;
%set(handles.detect,'userdata',{face,face0});
% set(handles.detect,'userdata',face0);
fprintf('Totally %d faces detected!\n', length(face));
fprintf('Totally %d faces detected!\n', length(face0));
axes(handles.axes1) ;
cla reset;
axes(handles.axes1);
 imshow(img);
 hold on;
for i = 1 : length(face)
    % Draw face rectangle on the image
    face_i = face{i};
    center = face_i.position.center;
    w = face_i.position.width / 100 * img_width;
    h = face_i.position.height / 100 * img_height;
    rectangle('Position', ...
        [center.x * img_width / 100 -  w/2, center.y * img_height / 100 - h/2, w, h+10], ...
        'Curvature', 0.4, 'LineWidth',2, 'EdgeColor', 'blue');
    age1=face_i.attribute.age.value;
    gender1=face_i.attribute.gender.value;
    race1=face_i.attribute.race.value;
end
axes(handles.axes2) ;
cla reset;
axes(handles.axes2);
 imshow(img0);
 hold on;
for i = 1 : length(face0)
    % Draw face rectangle on the image
    face_i = face0{i};
    center = face_i.position.center;
    w = face_i.position.width / 100 * img_width0;
    h = face_i.position.height / 100 * img_height0;
    rectangle('Position', ...
        [center.x * img_width0 / 100 -  w/2, center.y * img_height0 / 100 - h/2, w, h+10], ...
        'Curvature', 0.4, 'LineWidth',2, 'EdgeColor', 'blue');
    age2=face_i.attribute.age.value;
    gender2=face_i.attribute.gender.value;
    race2=face_i.attribute.race.value;
end
t=toc;
if strcmp(gender1,'Male')
    gender1='��';
else
    gender1='Ů';
end
if strcmp(gender2,'Male')
    gender2='��';
else
    gender2='Ů';
end
if strcmp(race1,'Asian')
    race1='���ޣ������ˣ�';
else if strcmp(race1,'White')
    race1='ŷ���������ˣ�';
else
    race1='���ޣ������ˣ�';
    end
end
if strcmp(race2,'Asian')
    race2='���ޣ������ˣ�';
else if strcmp(race2,'White')
    race2='ŷ���������ˣ�';
else
    race2='���ޣ������ˣ�';
    end
end
str1=sprintf('�������\n��ʱ %.4f��\none{ ���� %d��    �Ա� %s    ���� %s }\ntwo{ ���� %d��    �Ա� %s    ���� %s }\n',t,age1,gender1,race1,age2,gender2,race2);
set(handles.text4,'fontsize',10,'string',str1);
                
% --- Executes on button press in landmark.
function landmark_Callback(hObject, eventdata, handles)
% hObject    handle to landmark (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.text4,'string','');
tic;
global api img0 img rst rst0;
% fa=get(handles.detect,'userdata');
% face=fa{1};
% face0=fa{2};
% str=get(handles.select1,'userdata');
% str0=get(handles.select2,'userdata');
% rst = detect_file(api, str, 'all');
% rst0=detect_file(api,str0,'all');

img_width = rst{1}.img_width;
img_height = rst{1}.img_height;
img_width0 = rst0{1}.img_width;
img_height0 = rst0{1}.img_height;
face = rst{1}.face;
face0=rst0{1}.face;
% [img_width,img_height]=size(img);
% [img_width0,img_height0]=size(img0);
axes(handles.axes1) ;
cla reset;
axes(handles.axes1);
 imshow(img);
 hold on;
for i = 1 : length(face)
    % Draw face rectangle on the image
    face_i = face{i};    
    center = face_i.position.center;
    w = face_i.position.width / 100 * img_width;
    h = face_i.position.height / 100 * img_height;
    rectangle('Position', ...
        [center.x * img_width / 100 -  w/2, center.y * img_height / 100 - h/2, w, h+10], ...
        'Curvature', 0.4, 'LineWidth',2, 'EdgeColor', 'blue');
    % Detect facial key points
    rst2 = api.landmark(face_i.face_id, '83p');
    landmark_points = rst2{1}.result{1}.landmark;
    landmark_names = fieldnames(landmark_points);
    
    % Draw facial key points
    for j = 1 : length(landmark_names)
        pt = getfield(landmark_points, landmark_names{j});
        scatter(pt.x * img_width / 100, pt.y * img_height / 100, 'g.');
    end
end
axes(handles.axes2) ;
cla reset;
axes(handles.axes2);
 imshow(img0);
 hold on;
for i = 1 : length(face0)
    % Draw face rectangle on the image
    face_i = face0{i};
        center = face_i.position.center;
    w = face_i.position.width / 100 * img_width0;
    h = face_i.position.height / 100 * img_height0;
    rectangle('Position', ...
        [center.x * img_width0 / 100 -  w/2, center.y * img_height0 / 100 - h/2, w, h+10], ...
        'Curvature', 0.4, 'LineWidth',2, 'EdgeColor', 'blue');
    
    % Detect facial key points
    rst3 = api.landmark(face_i.face_id, '83p');
    landmark_points = rst3{1}.result{1}.landmark;
    landmark_names = fieldnames(landmark_points);
    
    % Draw facial key points
    for j = 1 : length(landmark_names)
        pt = getfield(landmark_points, landmark_names{j});
        scatter(pt.x * img_width0 / 100, pt.y * img_height0 / 100, 'g.');
    end
end
t=toc;
str1=sprintf('������������\n��ʱ %.4f��\n��������� 83��\n',t);
set(handles.text4,'fontsize',10,'string',str1);

% --- Executes on button press in compare.
function compare_Callback(hObject, eventdata, handles)
% hObject    handle to compare (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.text4,'string','');
tic;
global api  rst rst0;
% fa=get(handles.detect,'userdata');
% face=fa{1};
% face0=fa{2};
% str=get(handles.select1,'userdata');
% str0=get(handles.select2,'userdata');
% rst = detect_file(api, str, 'all');
% rst0=detect_file(api,str0,'all');
% img_width = rst{1}.img_width;
% img_height = rst{1}.img_height;
% img_width0 = rst0{1}.img_width;
% img_height0 = rst0{1}.img_height;
face = rst{1}.face;
face0=rst0{1}.face;

face_i=face{1};
face_j=face0{1};
cmp_rst=api.face_compare(face_i.face_id,face_j.face_id);
cmp_i=cmp_rst{1};
similarity=cmp_i.similarity;
cmp_con=cmp_i.component_similarity;
rst_eye=cmp_con.eye;
rst_eyebrow=cmp_con.eyebrow;
rst_mouth=cmp_con.mouth;
rst_nose=cmp_con.nose;
fprintf('eye similarity is %.4f\n', rst_eye);
fprintf('eyebrow similarity is %.4f\n', rst_eyebrow);
fprintf('mouse similarity is %.4f\n', rst_mouth);
fprintf('nose similarity is %.4f\n', rst_nose);
fprintf('face similarity is %.4f\n', similarity);
t=toc;
str1=sprintf('�����Ա�\n��ʱ %.4f��\n�۾����ƶ� %.4f\nüë���ƶ� %.4f\n������ƶ� %.4f\n�������ƶ� %.4f\n�������ƶ� %.4f\n',t,rst_eye,rst_eyebrow,rst_mouth,rst_nose,similarity);
set(handles.text4,'fontsize',9,'string',str1);
% str1=['�����Ա���ʱ',num2str(t),'��',10,'eye similarity is ', num2str(rst_eye),10,...
%     'eyebrow similarity is ',num2str(rst_eyebrow),10,'mouse similarity is ', ...
%     num2str(rst_mouth),10,'nose similarity is ', num2str(rst_nose),10,...
%     'face similarity is ', num2str(similarity)];
% str1=['�����Ա���ʱ',num2str(t),'��',10,'�۾����ƶ�  ', num2str(rst_eye),10,...
%     'üë���ƶ�  ',num2str(rst_eyebrow),10,'������ƶ�  ', ...
%     num2str(rst_mouth),10,'�������ƶ�  ', num2str(rst_nose),10,...
%     '�������ƶ� ', num2str(similarity)];
% set(handles.text4,'string',str1);

% --- Executes on button press in onebutton.
function onebutton_Callback(hObject, eventdata, handles)
% hObject    handle to onebutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.text4,'string','');
tic;
global api img img0;
str=get(handles.select1,'userdata');
str0=get(handles.select2,'userdata');
rst = detect_file(api, str, 'all');
rst0=detect_file(api,str0,'all');
img_width = rst{1}.img_width;
img_height = rst{1}.img_height;
img_width0 = rst0{1}.img_width;
img_height0 = rst0{1}.img_height;
face = rst{1}.face;
face0=rst0{1}.face;
% [img_width,img_height]=size(img);
% [img_width0,img_height0]=size(img0);
axes(handles.axes1) ;
cla reset;
axes(handles.axes1);
imshow(img);
hold on;
for i = 1 : length(face)
    % Draw face rectangle on the image
    face_i = face{i};    
    center = face_i.position.center;
    w = face_i.position.width / 100 * img_width;
    h = face_i.position.height / 100 * img_height;
    rectangle('Position', ...
        [center.x * img_width / 100 -  w/2, center.y * img_height / 100 - h/2, w, h+10], ...
        'Curvature', 0.4, 'LineWidth',2, 'EdgeColor', 'blue');
    % Detect facial key points
    rst2 = api.landmark(face_i.face_id, '83p');
    landmark_points = rst2{1}.result{1}.landmark;
    landmark_names = fieldnames(landmark_points);
    
    % Draw facial key points
    for j = 1 : length(landmark_names)
        pt = getfield(landmark_points, landmark_names{j});
        scatter(pt.x * img_width / 100, pt.y * img_height / 100, 'g.');
    end
end
axes(handles.axes2) ;
cla reset;
axes(handles.axes2);
imshow(img0);
hold on;
for i = 1 : length(face0)
    % Draw face rectangle on the image
    face_i = face0{i};
        center = face_i.position.center;
    w = face_i.position.width / 100 * img_width0;
    h = face_i.position.height / 100 * img_height0;
    rectangle('Position', ...
        [center.x * img_width0 / 100 -  w/2, center.y * img_height0 / 100 - h/2, w, h+10], ...
        'Curvature', 0.4, 'LineWidth',2, 'EdgeColor', 'blue');
    
    % Detect facial key points
    rst3 = api.landmark(face_i.face_id, '83p');
    landmark_points = rst3{1}.result{1}.landmark;
    landmark_names = fieldnames(landmark_points);
    
    % Draw facial key points
    for j = 1 : length(landmark_names)
        pt = getfield(landmark_points, landmark_names{j});
        scatter(pt.x * img_width0 / 100, pt.y * img_height0 / 100, 'g.');
    end
end
face_i=face{1};
face_j=face0{1};
cmp_rst=api.face_compare(face_i.face_id,face_j.face_id);
cmp_i=cmp_rst{1};
similarity=cmp_i.similarity;
cmp_con=cmp_i.component_similarity;
rst_eye=cmp_con.eye;
rst_eyebrow=cmp_con.eyebrow;
rst_mouth=cmp_con.mouth;
rst_nose=cmp_con.nose;
t=toc;
str1=sprintf('�����Ա�\n��ʱ %.4f��\n�۾����ƶ� %.4f\nüë���ƶ� %.4f\n������ƶ� %.4f\n�������ƶ� %.4f\n�������ƶ� %.4f\n',t,rst_eye,rst_eyebrow,rst_mouth,rst_nose,similarity);
set(handles.text4,'fontsize',9,'string',str1);
% str1=['�����Ա���ʱ',num2str(t),'��',10,'�۾����ƶ�  ', num2str(rst_eye),10,...
%     'üë���ƶ�  ',num2str(rst_eyebrow),10,'������ƶ�  ', ...
%     num2str(rst_mouth),10,'�������ƶ�  ', num2str(rst_nose),10,...
%     '�������ƶ� ', num2str(similarity)];
% set(handles.text4,'string',str1);

 save_txt = fopen('output.txt','wt');
 fprintf(save_txt,'�����Ա���ʱ %.4f��\n', t);
 fprintf(save_txt,'eye similarity is %.4f\n', rst_eye);
fprintf(save_txt,'eyebrow similarity is %.4f\n', rst_eyebrow);
fprintf(save_txt,'mouth similarity is %.4f\n', rst_mouth);
fprintf(save_txt,'nose similarity is %.4f\n', rst_nose);
fprintf(save_txt,'face similarity is %.4f\n', similarity);
fclose(save_txt);


% --- Executes on button press in close.
function close_Callback(hObject, eventdata, handles)
% hObject    handle to close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
button=questdlg('�Ƿ�ȷ�Ϲر�','�ر�ȷ��','��','��','��');
if strcmp(button,'��')
    close(gcf);
    delete(hObject);
else
    return;
end


% --- Executes on button press in start.
function start_Callback(hObject, eventdata, handles)
% hObject    handle to start (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clc;
clear img img0 pre pre0;
global img img0 pre pre0;
axes(handles.axes1) ;
cla reset;
axes(handles.axes2) ;
cla reset;
str='.\image\lena.jpg';
pre=str;
pre0=str;
img=imread(str);
axes(handles.axes1);
imshow(img);
str0=str;
img0=imread(str0);
axes(handles.axes2);
imshow(img0);
set(handles.select1,'userdata',str);
set(handles.select2,'userdata',str0);
set(handles.text1,'String',str);
set(handles.text2,'String',str0);
set(handles.text4,'String','��ʼ����ע�����Ҫ����������⣬�������������������Աȡ�Ҳ����һ������');
