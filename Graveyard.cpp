//data_collector image_collector;
//image_collector.save_n_video_frames(100,"../Data/Video_Data/Crowded_Sea_Floor.mp4","../Data/Crowded_Sea_Floor_Images/",0,0);
//data_collector video_maker;
//video_maker.create_video_from_folder("../Data/Crowded_Sea_Floor_Annotated/","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");

//test_methods tests;
// Run first sift contrast tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    for(int i = 0; i < 11; i++){
//        tests.run_test(i);
//    }
//}

// Run first sift contrast tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    for(int i = 0; i < 11; i++){
//        tests.run_test(i);
//    }
//}

// Run first sift contrast tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    for(int i = 0; i < 11; i++){
//        tests.run_test(i);
//    }
//}

// Test if matching works
//string video_path = "../Data/Video_Data/Solo_Pillar.mkv";
//tests.change_video_path(video_path);
//tests.run_test(11);

//vector<string> video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/New_Pillar_Top.mkv","../Data/Video_Data/New_Pillar_Bottom.mkv"};
//vector<string> video_paths = {"../Data/Video_Data/New_Pillar_Top.mkv"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.run_test(12);
//}

//vector<string> video_paths = {"../Data/Video_Data/Tank_With_Pipe.mkv"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.run_test(12);
//}

// Run first sift edge tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(13);
//}

// Run first sift edge tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(13);
//}

// Run first sift edge tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(13);
//}

//sift_settings desired_settings = {1000,3,0.02,10,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/frame_normal","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//sift_settings desired_settings = {1000,3,0.02,5,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/edge_low_frame_normal_crowded","../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");
//desired_settings = {1000,3,0.02,30,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/edge_medium_frame_normal_crowded","../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");
//desired_settings = {1000,3,0.02,60,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/edge_high_frame_normal_crowded","../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");

// Run first sift layer tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(15);
//}

// Run first sift layer tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(15);
//}

// Run first sift layer tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(15);
//}

//sift_settings desired_settings = {1000,5,0.02,43,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/layers_frame_normal_crowded_2","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/layers_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/layers_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first sift sigma tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(16);
//}

// Run first sift sigma tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(16);
//}

// Run first sift sigma tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(16);
//}
//sift_settings desired_settings = {1000,3,0.02,43,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/sigma_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/sigma_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/sigma_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first sift precise upscale tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(17);
//}

// Run first sift precise upscale tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(17);
//}

// Run first sift precise upscale tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(17);
//}

// Run first orb scale factor tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(18);
//}

// Run first orb scale factor tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(18);
//}

// Run first orb scale tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(18);
//}

//orb_settings desired_settings = {1000,1.2,8,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//orb_settings desired_settings = {1000,1.6,8,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb levels tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(19);
//}

// Run first orb levels tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(19);
//}

// Run first orb levels on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(19);
//}

//orb_settings desired_settings = {1000,1.6,6,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//orb_settings desired_settings = {1000,1.6,16,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb edge threshold tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(20);
//}

// Run first orb edge threshold tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(20);
//}

// Run first orb edge threshold tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(20);
//}

//orb_settings desired_settings = {1000,1.6,6,0,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//desired_settings = {1000,1.6,16,80,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb patch size tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(21);
//    }

// Run first orb patch size tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(21);
//    }

// Run first orb patch size tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(21);
//    }


//    orb_settings desired_settings = {1000,1.6,6,2,0,2,2,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,80,0,2,80,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb start level tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(22);
//    }

// Run first orb start level tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(22);
//    }

// Run first orb start level tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(22);
//    }

//    orb_settings desired_settings = {1000,1.6,6,34,0,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,3,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_medium_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_medium_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_medium_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,6,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");


// Run first orb point in element tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(23);
//    }

// Run first orb points in element tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(23);
//    }

// Run first orb points in element tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(23);
//    }

//    orb_settings desired_settings = {1000,1.6,6,34,0,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,0,4,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");


// Run first orb FAST tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(24);
//    }

// Run first orb FAST tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(24);
//    }

// Run first orb FAST tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(24);
//    }

//    orb_settings desired_settings = {1000,1.6,6,34,0,2,34,10};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,0,2,34,32};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_medium_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_medium_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_medium_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,0,2,34,50};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");


// Run comparison on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(25);
//    }

// Run comparison on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(25);
//    }

// Run comparison on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(25);
//    }


//    vector<string> video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/New_Pillar_Top.mkv","../Data/Video_Data/New_Pillar_Bottom.mkv"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.run_test(26);
//    }


//Mat grayscaled_last_frame, grayscaled_frame;
//last_frame.frame = cvtColor(last_frame.frame, grayscaled_last_frame, CV_BGR2GRAY);
//frame.frame = cvtColor(frame.frame, grayscaled_frame, CV_BGR2GRAY);

//optical_flow_results old_data[buffer_size];



//        vector<vector<vector<keypoint_data>>> possible_clusters_front;
//        vector<vector<vector<keypoint_data>>> possible_clusters_back;
//        int temp_cluster_count = cluster_count;
//        for(int clusters_remaining = cluster_count; clusters_remaining > 1; clusters_remaining--){
//            cout << "Clusters remaining: " << clusters_remaining << endl;
//            // Find possible outer clusters
//            vector<vector<keypoint_data>> current_front_ranges;
//            vector<vector<keypoint_data>> current_back_ranges;
//            cout << "Creating possibility" << endl;
//            for(int i = 0; i <= sorted_keypoints.size()-temp_cluster_count; i++){
//                int index = sorted_keypoints.size()-temp_cluster_count-i+1;
//                vector<keypoint_data> outer_front;
//                copy(sorted_keypoints.begin(), sorted_keypoints.begin()+index, back_inserter(outer_front));
//                vector<keypoint_data> outer_back;
//                copy(sorted_keypoints.end() - index, sorted_keypoints.end(), back_inserter(outer_back));
//                current_front_ranges.push_back(outer_front);
//                current_back_ranges.push_back(outer_back);
//            }
//            cout << "All posibilities created" << endl;
//            possible_clusters_front.push_back(current_front_ranges);
//            possible_clusters_back.push_back(current_back_ranges);

//            // remove outer values since they are not needed
//            sorted_keypoints.erase(sorted_keypoints.begin());
//            sorted_keypoints.erase(sorted_keypoints.end());

//            clusters_remaining--;
//            temp_cluster_count -= 2;
//        }

//        cout << "Outer clusters found" << endl;
//        cout << sorted_keypoints.size() << " remaining datapoints" << endl;

//        // Add middle range if number of clusters are unewen
//        vector<vector<keypoint_data>> middle_ranges;
//        if(cluster_count % 2 != 0){
//            for(int i = 0; i <= sorted_keypoints.size()-cluster_count; i++){
//                int index = sorted_keypoints.size()-cluster_count-i+1;

//                vector<keypoint_data> front;
//                copy(sorted_keypoints.begin(), sorted_keypoints.begin()+index, back_inserter(front));
//                vector<keypoint_data> back;
//                copy(sorted_keypoints.end() - index, sorted_keypoints.end(), back_inserter(back));

//                middle_ranges.push_back(front);
//                middle_ranges.push_back(back);
//            }
//        }
//        cout << "Middle clusters found" << endl;

//        // Reset sorted_keypoints
//        sorted_keypoints = sorted_keypoints_stored;

//        // Test print of data
//        for(int i = 0; i < possible_clusters_front.size(); i++){
//            cout << "Cluster " << i << " possibilities" << endl;
//            for(int j = 0; j < possible_clusters_front[i].size(); j++){
//                cout << "[";
//                for(int k = 0; k < possible_clusters_front[i].at(j).size(); k++){
//                    cout << possible_clusters_front[i].at(j).at(k).velocity << " ";
//                }
//                cout << "]" << endl;
//            }
//        }

//        // Test print of data
//        for(int i = 0; i < possible_clusters_back.size(); i++){
//            cout << "Cluster " << (cluster_count-i) << " possibilities" << endl;
//            for(int j = 0; j < possible_clusters_back[i].size(); j++){
//                cout << "[";
//                for(int k = 0; k < possible_clusters_back[i].at(j).size(); k++){
//                    cout << possible_clusters_back[i].at(j).at(k).velocity << " ";
//                }
//                cout << "]" << endl;
//            }
//        }

//        // Test print of data
//        cout << "Cluster " << (possible_clusters_front.size()+1) << " possibilities" << endl;
//        for(int j = 0; j < middle_ranges.size(); j++){
//            cout << "[";
//            for(int k = 0; k < middle_ranges[j].size(); k++){
//                cout << middle_ranges[j].at(k).velocity << " ";
//            }
//            cout << "]" << endl;
//        }



//// test stuff
//cout << "Updated data position vectors" << endl;
//for(int i = 0; i < current_data.size(); i++){
//    cout << "Point " << current_data.at(i).id << ": ";
//    for(int j = 0; j < current_data.at(i).positions.size(); j++){
//        cout << current_data.at(i).positions.at(j) << " ";
//    }
//    cout << endl;
//}


// Perform histogram equalization to hopefully make both images more similar
//            Mat gray_top = finder.apply_grayscale(frame_top);
//            Mat gray_bottom = finder.apply_grayscale(frame_bottom);
//            Mat equalized_top, equalized_bottom;
//            equalizeHist(gray_top,equalized_top);
//            equalizeHist(gray_bottom,equalized_bottom);
//            cvtColor(gray_top,frame_top,COLOR_GRAY2BGR);
//            cvtColor(gray_bottom,frame_bottom,COLOR_GRAY2BGR);
// Rotate frames 90 degrees to allign pillar
//            rotate(frame_top, frame_top, ROTATE_90_CLOCKWISE);
//            rotate(frame_bottom, frame_bottom, ROTATE_90_CLOCKWISE);


//cout << "Top feature count: " << top_features.size() << endl;
//cout << "Bottom feature count: " << bottom_features.size() << endl;
//cout << "Features found" << endl;

//match_result matches = analyzer.get_flann_matches(top_descriptors, bottom_descriptors,number_of_matches, lowes_threshold);
//match_result matches = analyzer.get_brute_matches(top_descriptors, bottom_descriptors,number_of_matches,true);


// Plotting positions of match on same image
// Test print best results
//                for(int i = 0; i < matches.matches.size(); i++){
//                    if(matches.good_matches[i] == true){
//                        cout << i << endl;
//                        Mat test_image = frame_top.clone();
//                        Point2f keypoint_top = top_features[matches.matches[i].queryIdx].pt;
//                        Point2f keypoint_bottom = bottom_features[matches.matches[i].trainIdx].pt;
//                        cout << "top = (" << keypoint_top.x << ", " << keypoint_top.y << ")" << endl;
//                        cout << "bottom = (" << keypoint_bottom.x << ", " << keypoint_bottom.y << ")" << endl;
//                        circle(test_image,keypoint_top,5,Scalar(0,0,255),-1);
//                        circle(test_image,keypoint_bottom,5,Scalar(0,255,0),-1);
//                        imshow("current point",test_image);
//                        waitKey(0);
//                    }
//                }

//                cout << matches.good_matches.size() << endl;
//                cout << top_features.size() << endl;
//                cout << bottom_features.size() << endl;
//                Mat img_matches;
//                drawMatches(frame_top, top_features, frame_bottom, bottom_features, matches.matches, img_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                Mat img_keypoints_top;
//                drawKeypoints(frame_top, top_features, img_keypoints_top );
//                Mat img_keypoints_bottom;
//                drawKeypoints(frame_bottom, bottom_features, img_keypoints_bottom );
//                imshow("matches",img_matches);
//                imshow("Top features",img_keypoints_top);
//                imshow("Bottom features",img_keypoints_bottom);
//                waitKey(0);
//                if(n_matches != top_features.size()){
//                    cout << matches.good_matches.size() << endl;
//                    cout << top_features.size() << endl;
//                    cout << bottom_features.size() << endl;
//                    Mat img_matches;
//                    drawMatches(frame_top, top_features, frame_bottom, bottom_features, matches.matches, img_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                    vector<DMatch> kept_matches;
//                    for(int i = 0; i < matches.matches.size(); i++){
//                        if(matches.good_matches[i] == true){
//                            kept_matches.push_back(matches.matches[i]);
//                        }
//                    }
//                    Mat img_kept_matches;
//                    drawMatches(frame_top, top_features, frame_bottom, bottom_features, kept_matches, img_kept_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                    imshow("matches that went wrong",img_matches);
//                    imshow("matches that survived",img_kept_matches);
//                    waitKey(0);
//                }


//                // Add found feature to currently alive features
//                current_top_features.insert(current_top_features.end(), top_features.begin(), top_features.end());
//                current_bottom_features.insert(current_bottom_features.end(), bottom_features.begin(), bottom_features.end());


// The manual test
//                int approved_count = 0;
//                for(int i = 0; i < kept_matches.size();i++){
//                    Mat img_temp;
//                    vector<DMatch> current_matches;
//                    current_matches.push_back(kept_matches[i]);
//                    drawMatches(frame_top, top_features, frame_bottom, bottom_features, current_matches, img_temp, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                    imshow("Current match",img_temp);
//                    waitKey(1);
//                    cout << "y -> approved, n -> not approved" << endl;
//                    string answer;
//                    while(true){
//                        cin >> answer;
//                        if(answer == "y" || answer == "n"){
//                            cout << "I know that letter" << endl;
//                            break;
//                        }
//                        else{
//                            cout << "Unrecognised input" << endl;
//                        }
//                    }
//                    if(answer == "y"){
//                        approved_count++;
//                    }
//                }


// -- PERFORM OPTICAL FLOW --
//optical_flow_results top_flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_top_features), last_top_frame, frame_top);
//optical_flow_results bottom_flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_bottom_features), last_bottom_frame, frame_bottom);

//// -- UPDATE DATA BASED ON OPTICAL FLOW --
//current_top_data = analyzer.flow_update(current_top_data,top_flow_results);
//current_bottom_data = analyzer.flow_update(current_bottom_data,bottom_flow_results);
//vector<vector<keypoint_data>> cleaned_data = analyzer.ensure_matching_performance(current_top_data, current_bottom_data);
//current_top_data = cleaned_data.at(0);
//current_bottom_data = cleaned_data.at(1);

// -- KEEP ONLY FEATURES THAT DO WELL IN BOTH FRAMES --


//// increase count of frames processed since last feature matching process
//frames_since_match++;

//// -- CHECK IF FLOW LIMIT IS REACHED --
//if(frames_since_match >= max_frame_gap-1){
//    // -- CALCULATE VELOCITY --
//    current_top_data = analyzer.determine_velocity(current_top_data, fps, max_frame_gap);
//    current_bottom_data = analyzer.determine_velocity(current_bottom_data, fps, max_frame_gap);
//    // -- AVERAGE VELOCITIES --
//    vector<float> velocities = analyzer.combine_velocities(current_top_data,current_bottom_data);
//    current_top_data = analyzer.add_velocities(current_top_data,velocities);
//    current_bottom_data = analyzer.add_velocities(current_bottom_data,velocities);
//    // -- FILTER VELOCITIES
//    current_top_data = analyzer.velocity_filter(current_top_data,percentile,flow_threshold);
//    current_bottom_data = analyzer.velocity_filter(current_bottom_data,percentile,flow_threshold);
//    // -- VISUALIZE RESULTS --
//    Mat top_viz,bottom_viz;
//    // Mark points
//    Mat circle_top_frame = visualizer.mark_keypoints(current_top_data,frame_top);
//    Mat circle_bottom_frame = visualizer.mark_keypoints(current_bottom_data,frame_bottom);
//    // Draw lines for alive features
//    Mat line_top_frame = visualizer.mark_lines(current_top_data,frame_top);
//    Mat line_bottom_frame = visualizer.mark_lines(current_bottom_data,frame_bottom);
//    // Combine mask and frame
//    add(line_top_frame,circle_top_frame,top_viz);
//    add(line_bottom_frame,circle_bottom_frame,bottom_viz);
//    // Add velocity text
//    top_viz = visualizer.mark_velocity(current_top_data,top_viz);
//    bottom_viz = visualizer.mark_velocity(current_bottom_data,bottom_viz);
//    imshow("top vel", top_viz);
//    imshow("bottom vel", bottom_viz);
//    waitKey(0);
//    frames_since_match = 0;
//}

//// -- UPDATE SURVIVING FEATURES --
//current_top_features = analyzer.get_keypoints(current_top_data);
//current_bottom_features = analyzer.get_keypoints(current_bottom_data);


//        for(int row = 0; row < A_T.rows; row++){
//            for(int col = 0; col < A_T.cols; col++){
//                A_T_eigen << A_T.at<double>(row,col);
//                A_eigen << A.at<double>(row,col);
//            }
//        }

// Stop when window is closed (else just continue forever and ever)
//        while(!frame_3d.wasStopped()){
//        }


// -- WARNING: ENTERING TEST AREA. THINGS ARE MESSY DOWN HERE --
// For test purposes i disable find features here so we can analyse the features using optical flow

//// TEST 3D ESTIMATION
//visualizer.visualize_3d_points(point_estimates);

//// TEST SLIC
//Mat viz_top = visualizer.mark_super_pixel_borders(frame_top,top_slic_results);
//imshow("top super pixels", viz_top);
//waitKey(0);

//Mat viz_median = visualizer.mark_super_pixels(frame_top,top_slic_results);
//imshow("Medians", viz_median);
//waitKey(0);

//imwrite("superpixel_border.jpg",viz_top);
//imwrite("superpixel.jpg",viz_median);

//// Test show surviving features
//Mat img_keypoints_top = visualizer.mark_keypoints(current_top_data,frame_top);
//Mat img_keypoints_bottom = visualizer.mark_keypoints(current_bottom_data,frame_top);
//imshow("Top features",img_keypoints_top);
//imshow("Bottom features",img_keypoints_bottom);
//waitKey(0);

//// Show surviving matches
//int survivor_count = analyzer.match_survivor_count(matches);
//cout << "Surviving features: " << survivor_count << endl;

//// Get surviving matches
//vector<DMatch> kept_matches = analyzer.get_surviving_matches(matches);

//// Visualize surviving matches
//Mat img_kept_matches;
//drawMatches(frame_top, top_features, frame_bottom, bottom_features, kept_matches, img_kept_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//imshow("matches that survived",img_kept_matches);
//imwrite("survivors.jpg",img_kept_matches);
//waitKey(0);

//                for(int j = 0; j < current_top_data.size(); j++){
//                    for(int j2= j; j2 < updated_matches.good_matches.size(); j2++){
//                        if(current_top_data.at(j).valid == false && updated_matches.good_matches.at(j2) == true){
//                            updated_matches.good_matches.at(j2) = false;
//                            break;
//                        }
//                    }
//                }


//super_pixel_frame updated_data = data;
//try{
//    // Initialize needed classes
//    feature_analyzer analyzer; // Analyzes features

//    // Get superpixel identifier
//    //vector<Vec3b> medians = analyzer.get_superpixel_medians(data,frame);
//    vector<Vec3b> medians;

//    // Initialize vector of labels similar to each other label
//    vector<vector<int>> label_clusters;

//    // Find distance from all medians to all other medians
//    for(int i = 0; i < medians.size(); i++){
//        int val_1 = medians.at(i)[0];
//        int val_2 = medians.at(i)[1];
//        int val_3 = medians.at(i)[2];
//        int comp_1,comp_2,comp_3;
//        for(int j = (i+1); j < medians.size(); j++){
//            comp_1 = medians.at(j)[0];
//            comp_2 = medians.at(j)[1];
//            comp_3 = medians.at(j)[2];
//        }
//        int diff_1 = abs(val_1-comp_1);
//        int diff_2 = abs(val_2-comp_2);
//        int diff_3 = abs(val_3-comp_3);
//        int total_diff = diff_1+diff_2+diff_3;
//        cout << total_diff << endl;
//    }
//    // INSTEAD MAYBE JUST CLUSTER WITH K-MEANS

//    // Go through every pixel to find its membership
//    for(int row = 0; row < frame.rows; row++){
//        for(int col = 0; col < frame.cols; col++){

//        }
//    }
//}
//catch(const exception& error){
//    cout << error.what() << endl;
//}
//return updated_data;

//vector<vector<int>> final_clusters = {};
//vector<int> used_clusters = {};

//for(int cluster = 0; cluster < clusters.size(); cluster++){
//    // Only continue if cluster not already used
//    int used_count = count(used_clusters.begin(), used_clusters.end(), cluster);
//    if(used_count == 0){
//        vector<int> temp_cluster = clusters.at(cluster);
//        used_clusters.push_back(cluster);
//        // Match wih all other clusters
//        for(int comp_cluster = cluster+1; comp_cluster < clusters.size(); comp_cluster++){
//            int second_used_count = count(used_clusters.begin(), used_clusters.end(), comp_cluster);
//            if(second_used_count == 0){
//                int count_comp = 0;
//                bool valid = true;
//                // Go through elements and count them in second cluster
//                for(int element = 0; element < clusters.at(cluster).size(); element++){
//                    // ensure no duplicates
//                    if(count(used_clusters.begin(), used_clusters.end(), clusters.at(cluster).at(element)) > 0){ // ISSUE HAPPENS HERE (More elements in previous cluster than new one)
//                        valid = false;
//                    }
//                    int superpixel = clusters.at(cluster).at(element);
//                    int count_comp_temp = count(clusters.at(comp_cluster).begin(), clusters.at(comp_cluster).end(),superpixel);
//                    if(count_comp_temp > count_comp){
//                        count_comp = count_comp_temp;
//                    }

//                }
//                if(count_comp > 0 && valid == true){
//                    temp_cluster.insert(temp_cluster.end(),clusters.at(comp_cluster).begin(),clusters.at(comp_cluster).end());
//                    // add all labels in comp cluster to used clusters
//                    used_clusters.push_back(comp_cluster);
//                    for(int k = 0; k < clusters.at(comp_cluster).size();k++){
//                        used_clusters.push_back(clusters.at(comp_cluster).at(k));
//                    }
//                }
//            }
//        }
//        if(temp_cluster.size() == 0){
//            cout << "Empty cluster accepted: " << final_clusters.size() << endl;
//        }
//        // Remove duplicates from vector
//        sort(temp_cluster.begin(), temp_cluster.end());
//        auto it = unique(temp_cluster.begin(), temp_cluster.end());
//        temp_cluster.erase(it, temp_cluster.end());
//        // Add cluster to final
//        final_clusters.push_back(temp_cluster);
//    }
//}


//cout << "No common elements. Lets check." << endl;
//for(int element = 0; element < clusters.at(cluster).size(); element++){
//    cout << clusters.at(cluster).at(element) << " ";
//}
//cout << endl;
//for(int element = 0; element < clusters.at(comp_cluster).size(); element++){
//    cout << clusters.at(comp_cluster).at(element) << " ";
//}
//cout << endl;
//return data;

//            // Add matches if found in other matches
//            vector<int>::iterator iterator = closest_match.begin();
//            vector<int> match_indexes;
//            while((iterator = find_if(iterator, closest_match.end(),[&superpixel](int x){return x == superpixel;} )) != closest_match.end())
//            {
//                match_indexes.push_back(distance(closest_match.begin(), iterator));
//                iterator++;
//            }
//            // Add all match indexes to cluster if not already present.
//            for(int match = 0; match < match_indexes.size(); match++){
//                // Ensure not already added
//                int cluster_count = count(temp_cluster.begin(), temp_cluster.end(), match_indexes.at(match));
//                if(cluster_count == 0){
//                    temp_cluster.push_back(match_indexes.at(match));
//                }
//            }

//        cout << "Clusters after combining: " << final_clusters.size() << endl;
//        cout << "Number of clusters: " << final_clusters.size() << endl;
//        for(int i = 0; i < final_clusters.size(); i++){
//            cout << "Cluster " << i << " size: " << final_clusters.at(i).size() << endl;
//            for(int j = 0; j < final_clusters.at(i).size();j++){
//                cout << final_clusters.at(i).at(j) << ", ";
//            }
//            cout << endl;
//        }
        // Update superpixels
//        cout << "The dreaded 20: ";
//        for(int i = 0; i < final_clusters.at(20).size(); i++){
//            cout << final_clusters.at(20).at(i) << " ";
//        }
//        cout << endl;


// -- FIX CAMERA MATRIX BASED ON RESIZING --
//        cam_top.resize_intrensic(frame_scale_factor,CENTER_TOP_LEFT);
//        cam_bottom.resize_intrensic(frame_scale_factor,CENTER_TOP_LEFT);

//        // -- IF NO RESIZING --
//        cam_top.create_intrinsic_matrix();
//        cam_bottom.create_intrinsic_matrix();

// -- VISUALIZE CAM DATA --
//        cam_top.vizualize_cam_info(TOP_CAM);
//        cam_bottom.vizualize_cam_info(BOTTOM_CAM);

// -- OR FIX CAMERA MATRIX USING OPENCV --
//        cam_top.resize_intrensic_opencv();
//        cam_bottom.resize_intrensic_opencv();



// Convert to float and make smaller
//disparity.convertTo(disparity,CV_8U,255/(num_disparities*16.0));
//disparity.convertTo(disparity,CV_32F);
//cout << "After float convert: " << disparity.at<float>(0,8) << endl;

// Bingus disparity visualization
// -- Remove disparities below 0 and find min and max
//            double min_val;
//            double max_val;
//            for(int row = 0; row < disparity.rows; row++){
//                for(int col = 0; col < disparity.cols; col++){
//                    if(disparity.at<double>(row,col) < 0.0){
//                        disparity.at<double>(row,col) = 0.0;
//                    }
//                    else if(col == 8 && row == 0){
//                        cout << "A normal one at " << row << ", " << col << ": " << disparity.at<double>(row,col) << endl;
//                    }
//                    if(row == 0){
//                        min_val = disparity.at<double>(row,col);
//                        max_val = disparity.at<double>(row,col);
//                    }
//                    if(disparity.at<double>(row,col) > max_val){
//                        max_val = disparity.at<double>(row,col);
//                    }
//                    if(disparity.at<double>(row,col) < min_val){
//                        min_val = disparity.at<double>(row,col);
//                    }
//                }
//            }
// -- Normalize
//            double min_val;
//            double max_val;
//            Point min_location;
//            Point max_location;
//            minMaxLoc(disparity,&min_val,&max_val,&min_location,&max_location);
//            cout << "min: " << min_val << endl;
//            cout << "max: " << max_val << endl;
//            disparity = (disparity-min_val)/(max_val-min_val);
//            cout << "New val: " << disparity.at<double>(0,8) << endl;
//            disparity.convertTo(disparity,CV_8U,1.0/255.0,0);
//            cout << "New val after convert: " << disparity.at<int>(0,8) << endl;
//cout << disparity.at<double>(0,0) << endl;
//disparity = 255.0f*disparity;
//cout << disparity.at<double>(0,0) << endl;
//disparity.convertTo(disparity,CV_8U);

// -- Make color map
//            Mat im_color;
//            applyColorMap(disparity,im_color,COLORMAP_JET);

// Convert disparity values to CV_32F from CV_16S
//disparity.convertTo(disparity,CV_32F,1.0);
//disparity.convertTo(disparity,CV_8U,255/(num_disparities*16.0));

// Scale down disparity
//disparity = disparity/16.0f;
//disparity = (disparity/16.0f-(float)minDisparity)/((float)numDisparities);
