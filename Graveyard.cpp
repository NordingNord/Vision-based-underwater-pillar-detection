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

//// Calcualte fundamental and essential matrix
//Mat essential_matrix = cam_storage.calculate_essential();
//Mat fundamental_matrix = cam_storage.calculate_fundamental();

//            rotate(left_frame, left_frame, ROTATE_90_CLOCKWISE);
//            rotate(right_frame, right_frame, ROTATE_90_CLOCKWISE);

// Initialise fundamental rectification if first frame
//            if(frame_count == 1){
//                cam_storage.prepare_rectify_fundamental(left_frame,right_frame);
//            }


//if(frame_count == 100){
//    imwrite("left.png",left_frame);
//    imwrite("right.png",right_frame);
//    left_frame = frames.at(0);
//    right_frame = frames.at(1);
//    cout << "Size left: " << left_frame.size() << endl;
//    cout << "Size right: " << right_frame.size() << endl;
//    combined;
//    hconcat(left_frame,right_frame,combined);
//    for(int j = 0; j < combined.rows; j+=16){
//        if(j/16 == 37){
//            line(combined,Point(0,j),Point(combined.cols,j),Scalar(0,255,0),2,8);
//        }
//    }
//    resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
//    imshow("Rectified frames",combined);
//    imwrite("Rectified.png", combined);
//    waitKey(0);
//    cout << "done" << endl;
//}

//// Show frames
//Mat combined;
//hconcat(left_frame,right_frame,combined);
//resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
//imshow("Original frames",combined);
//waitKey(0);

//// show transposed frames
//combined;
//hconcat(left_frame,right_frame,combined);
//resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
//imshow("Transposed frames",combined);
//waitKey(0);

//    // Test preprocess //
//    camera_handler cam_handler_pre("../Data/Video_Data/Preprocessed_Solo_Pillar.mkv");
//    camera_handler cam_handler("../Data/Video_Data/Solo_Pillar.mkv");

//    feature_finder finder(0);
//    data_visualization visualizer;
//    feature_analyzer analyzer;
//    vector<KeyPoint> current_features;
//    vector<keypoint_data> current_data;
//    vector<KeyPoint> current_features_pre;
//    vector<keypoint_data> current_data_pre;
//    int index = 0;

//    while(true){
//        Mat frame = cam_handler.get_frame();
//        Mat frame_pre = cam_handler_pre.get_frame();

//        // Break if no more frames
//        if(frame_pre.empty()){
//            cout << "Reached end of video stream" << endl;
//            break;
//        }
//        current_features = finder.find_features(frame);
//        current_data = analyzer.convert_to_data(current_features);
//        vector<Scalar> colours = visualizer.generate_random_colours(current_features.size());
//        current_data = analyzer.insert_data(current_data, colours);
//        Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
//        Mat scaled;
//        resize(circle_frame,scaled, Size(), 0.5, 0.5, INTER_LINEAR);


//        current_features_pre = finder.find_features(frame_pre);
//        current_data_pre = analyzer.convert_to_data(current_features_pre);
//        colours = visualizer.generate_random_colours(current_features_pre.size());
//        current_data_pre = analyzer.insert_data(current_data_pre, colours);
//        Mat circle_frame_pre = visualizer.mark_keypoints(current_data_pre,frame_pre);
//        Mat scaled_pre;
//        resize(circle_frame_pre,scaled_pre, Size(), 0.5, 0.5, INTER_LINEAR);
//        string path = "../Data/preprocess_images/" + to_string(index) +".jpg";
//        imwrite(path, scaled);
//        string path_pre = "../Data/preprocess_images/" + to_string(index) +"_pre.jpg";
//        imwrite(path_pre, scaled_pre);
//        imshow("frame", scaled);
//        imshow("preprocessed frame", scaled_pre);
//        waitKey(0);
//    }

//    detection.multicam_pipeline(top_video,bottom_video,METHOD_AKAZE,MATCH_BRUTE_CROSS,FILTER_RANSAC,MODE_MANUAL, ORIGINAL);

//    detection.get_detection_data("/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full/Homomorphic_video.mkv", 0, 0, false);

//    // Testing optical flow //
//    obstacle_detection detection;
//    detection.perform_optical_flow("../Data/Video_Data/Heavy_Blur_New_Pillar_Top.mkv",0, true, ON_FRAME, "preprocess test",0); // 0 -> ORB 2 -> SIFT last int is gap
//    detection.perform_optical_flow("../Data/Video_Data/Solo_Pillar.mkv",0, true, ON_FRAME, "gap_test",0);
//    feature_analyzer test;
//    vector<keypoint_data> keypoints;
//    vector<float> velocities = {4,5,9,10};
//    for(int i = 0; i < velocities.size(); i++){
//        keypoint_data keypoint;
//        keypoint.velocity = velocities[i];
//        keypoints.push_back(keypoint);
//    }
//    test.Jenks_Natural_Breaks_Clustering(keypoints, 2);

    // Test kalman filter
//    vector<float> x = {1,2,3,4,6};
//    vector<float> y = {1,1,5,-1,1};
//    vector<Point2f> results;
//    feature_analyzer test;
//    test.init_kalman(4,2,0,0,0);
//    for(int i = 0; i < x.size(); i++){
//        Point2f result = test.predict_kalman();
//        results.push_back(result);
//        test.correct_kalman(x[i],y[i]);
//        cout << result.x << ", " << result.y << endl;
//    }


//    // Test median filter
//    camera_handler cam_handler("../Data/Video_Data/New_Pillar_Top.mkv");
//    preprocessing preprocessor;
//    while(true){
//        Mat frame = cam_handler.get_frame();
//        // Break if no more frames
//        if(frame.empty()){
//            cout << "Reached end of video stream" << endl;
//            break;
//        }
//        Mat result = preprocessor.median_filter(frame,11);
//        imshow("Blurred frame",result);
//        imshow("original frame",frame);
//        waitKey(0);
//    }

//string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full/Homomorphic_video.mkv";
//string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Homomorphic_video.mkv";
//string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full/Resized_video.mkv";
//string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Resized_video.mkv";
//string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_4_full/Homomorphic_video.mkv";
//string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_4_full_bottom/Homomorphic_video.mkv";

// Test simplified optical flow
//    obstacle_detection detection;

//    detection.estimation_3d(bottom_video,top_video);


// Write shorter video
//VideoCapture cap_bottom(bottom_video);
//VideoCapture cap_top(top_video);

//int frame_width = cap_bottom.get(cv::CAP_PROP_FRAME_WIDTH);
//int frame_height = cap_bottom.get(cv::CAP_PROP_FRAME_HEIGHT);


//VideoWriter video_bottom("/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height));
//VideoWriter video_top("/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height));
//Mat frame_bottom, frame_top;
//int cap_start = 6930;
//int count = 0;
//while(true){
//    // Read frames
//    cap_bottom >> frame_bottom;
//    cap_top >> frame_top;
//    count++;
//    if(count >= cap_start){
//        video_bottom.write(frame_bottom);
//        video_top.write(frame_top);
//    }
//    if(frame_bottom.empty() || frame_top.empty()){
//        break;
//    }
//}

//// Prepare triangulated matrix
//Mat estimated_3d_matrix;

//// Run triangulation
//triangulatePoints(first_projection, second_projection,first_points,second_points,estimated_3d_matrix);
//cout << second_points.size() << endl;


//Point3f temp_3d;
////homogeneous so we un homogenize it -> homogenous to cartesian space.
//temp_3d.x = estimated_3d_matrix.at<double>(0,point_index)/estimated_3d_matrix.at<double>(3,point_index);
//temp_3d.y = estimated_3d_matrix.at<double>(1,point_index)/estimated_3d_matrix.at<double>(3,point_index);
//temp_3d.z = estimated_3d_matrix.at<double>(2,point_index)/estimated_3d_matrix.at<double>(3,point_index);
//// push to output
//triangulated_points.push_back(temp_3d);
//cout << "(" << temp_3d.x << ", " << temp_3d.y << ", " << temp_3d.z << ")" << endl;
//cout << "(" << second_points.at(point_index).x << ", " << second_points.at(point_index).y << ")" << endl;
//cout << "(" << first_points.at(point_index).x << ", " << first_points.at(point_index).y << ")" << endl;
//cout << estimated_3d_matrix.at<double>(3,point_index) << endl;

//// Get max and min depth
//Mat depth_channel;
//extractChannel(depth_map,depth_channel,2);

//float min_depth = 0.0;
//float max_depth = 0.0;
//vector<float> depths;
//bool init = true;
//for(int row_index = 0; row_index < depth_channel.rows; row_index++){
//    for(int col_index = 0; col_index < depth_channel.cols; col_index++){
//        float depth = depth_channel.at<float>(Point(col_index,row_index));
//        if(isinf(depth) == false && isinf(depth) == false && isinf(depth) == false && isnan(depth) == false && isnan(depth) == false && isnan(depth) == false){
//            if(init == true){
//                min_depth = depth;
//                max_depth = min_depth;
//                init = false;
//                cout << "First depth: " << depth << endl;
//            }
//            else{
//                if(depth < min_depth){
//                    min_depth = depth;
//                }
//                if(depth > max_depth){
//                    max_depth = depth;
//                }
//            }
//            depths.push_back(depth);
//        }
//    }
//}

//// Find 75 percentile
//sort(depths.begin(), depths.end());
//cout << depths.at(depths.size()-1) << endl;
//int index = (0.95*depths.size()); // If percentile is inbetween two values, the lowest is taken.
//float percentile_value = depths.at(index);
//max_depth = percentile_value;




// ----------------------------------------------------------------------------------------------------------------------------------

//// Split contour using the found lines
//// find best direction for best line
//Vec4i first_line;
//Vec4i last_line;
//bool first_found = false;
//int steps_since_change = 0;
//if(best_angle*180/M_PI < abs(best_angle*180/M_PI-90)){
//    // vertical edge
//    cout << "Vertical" << endl;


//    // create directional linemasks
//    Mat left_mask = Mat::zeros(mask.size(),CV_8U);
//    Mat right_mask = Mat::zeros(mask.size(),CV_8U);
//    line(left_mask,Point(best_line[0]-1,best_line[1]),Point(best_line[2]-1,best_line[3]),255,3,LINE_AA); // Move one pixel left
//    line(right_mask,Point(best_line[0]+1,best_line[1]),Point(best_line[2]+1,best_line[3]),255,3,LINE_AA); // Move one pixel right

//    // Find direction with most matches
//    Mat left_match_mat = left_mask & mask;
//    Mat right_match_mat = right_mask & mask;
//    int left_matches = countNonZero(left_match_mat);
//    int right_matches = countNonZero(right_match_mat);

//    int direction_sign = 1;
//    int max_index = mask.cols;
//    if(left_matches > right_matches){
//        cout << "Left" << endl;
//        // set last line as border if no other border is found
//        last_line[0] = 0;
//        last_line[1] = 0;
//        last_line[2] = 0;
//        last_line[3] = frame.rows-1;
//        direction_sign = -1;
//        max_index = min(best_line[0],best_line[2]);
//    }
//    else{
//        cout << "Right" << endl;
//        last_line[0] = frame.cols-1;
//        last_line[1] = 0;
//        last_line[2] = frame.cols-1;
//        last_line[3] = frame.rows-1;
//    }
//    int most_matches = 0;//max(left_matches,right_matches);
//    int last_matches = 0;
//    for(int x_index = 0; x_index < max_index; x_index++){
//        // Get new mask
//        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
//        line(new_mask,Point(best_line[0]+(x_index*direction_sign),best_line[1]),Point(best_line[2]+(x_index*direction_sign),best_line[3]),255,3,LINE_AA);

//        // Count matches
//        Mat match_mask = new_mask & mask;
//        int match_count = countNonZero(match_mask);
//        // Check if better than previous
//        if(most_matches < match_count && first_found == false){
//            most_matches = match_count;
//            first_line[0] = best_line[0]+(x_index*direction_sign);
//            first_line[1] = best_line[1];
//            first_line[2] = best_line[2]+(x_index*direction_sign);
//            first_line[3] = best_line[3];
//            steps_since_change = 0;
//        }
//        else{
//            steps_since_change++;
//            if(steps_since_change >= 5){
//                first_found = true;
//            }
//        }
//        // Check if begining to decline
//        if(match_count < last_matches*0.75|| x_index+1 == max_index){
//            cout << match_count << endl;
//            cout << last_matches << endl;
//            last_line[0] = best_line[0]+((x_index-1)*direction_sign);
//            last_line[1] = best_line[1];
//            last_line[2] = best_line[2]+((x_index-1)*direction_sign);
//            last_line[3] = best_line[3];
//            break;
//        }
//        last_matches = match_count;
//    }
//}
//else{
//    // horizontal edge
//    cout << "Horizontal" << endl;

//    // create directional linemasks
//    Mat up_mask = Mat::zeros(mask.size(),CV_8U);
//    Mat down_mask = Mat::zeros(mask.size(),CV_8U);
//    line(up_mask,Point(best_line[0],best_line[1]-1),Point(best_line[2],best_line[3]-1),255,3,LINE_AA); // Move one pixel up
//    line(down_mask,Point(best_line[0],best_line[1]+1),Point(best_line[2],best_line[3]+1),255,3,LINE_AA); // Move one pixel down

//    // Find direction with most matches
//    Mat up_match_mat = up_mask & mask;
//    Mat down_match_mat = down_mask & mask;
//    int up_matches = countNonZero(up_match_mat);
//    int down_matches = countNonZero(down_match_mat);

//    int direction_sign = 1;
//    int max_index = mask.rows;
//    if(up_matches > down_matches){
//        cout << "Top" << endl;
//        last_line[0] = 0;
//        last_line[1] = 0;
//        last_line[2] = frame.cols-1;
//        last_line[3] = 0;
//        direction_sign = -1;
//        max_index = min(best_line[1],best_line[3]);
//    }
//    else{
//        last_line[0] = 0;
//        last_line[1] = frame.rows-1;
//        last_line[2] = frame.cols-1;
//        last_line[3] = frame.rows-1;
//        cout << "Bottom" << endl;
//    }
//    int most_matches = 0;
//    int last_matches = 0;
//    for(int y_index = 0; y_index < max_index; y_index++){
//        // Get new mask
//        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
//        line(new_mask,Point(best_line[0],best_line[1]+(y_index*direction_sign)),Point(best_line[2],best_line[3]+(y_index*direction_sign)),255,3,LINE_AA);
//        // Count matches
//        Mat match_mask = new_mask & mask;
//        int match_count = countNonZero(match_mask);
//        // Check if better than previous
//        if(most_matches < match_count && first_found == false){
//            most_matches = match_count;
//            first_line[0] = best_line[0];
//            first_line[1] = best_line[1]+(y_index*direction_sign);
//            first_line[2] = best_line[2];
//            first_line[3] = best_line[3]+(y_index*direction_sign);
//            steps_since_change = 0;
//            cout << "winner" << endl;
//        }
//        else{
//            steps_since_change++;
//            if(steps_since_change >= 2){
//                first_found = true;
//            }
//        }
//        // Check if begining to decline
//        if(match_count < last_matches*0.75 || y_index+1 == max_index){
//            last_line[0] = best_line[0];
//            last_line[1] = best_line[1]+((y_index-1)*direction_sign);
//            last_line[2] = best_line[2];
//            last_line[3] = best_line[3]+((y_index-1)*direction_sign);
//            break;
//        }
//        last_matches = match_count;
//    }
//}

//// find best direction for second best line
//Vec4i second_first_line;
//Vec4i second_last_line;
//steps_since_change = 0;
//first_found = false;
//if(second_best_angle*180/M_PI < abs(second_best_angle*180/M_PI-90)){
//    // vertical edge
//    cout << "Vertical" << endl;

//    // create directional linemasks
//    Mat left_mask = Mat::zeros(mask.size(),CV_8U);
//    Mat right_mask = Mat::zeros(mask.size(),CV_8U);
//    line(left_mask,Point(second_best_line[0]-1,second_best_line[1]),Point(second_best_line[2]-1,second_best_line[3]),255,3,LINE_AA); // Move one pixel left
//    line(right_mask,Point(second_best_line[0]+1,second_best_line[1]),Point(second_best_line[2]+1,second_best_line[3]),255,3,LINE_AA); // Move one pixel right

//    // Find direction with most matches
//    Mat left_match_mat = left_mask & mask;
//    Mat right_match_mat = right_mask & mask;
//    int left_matches = countNonZero(left_match_mat);
//    int right_matches = countNonZero(right_match_mat);

//    int direction_sign = 1;
//    int max_index = mask.cols;
//    if(left_matches > right_matches){
//        cout << "Left" << endl;
//        second_last_line[0] = 0;
//        second_last_line[1] = 0;
//        second_last_line[2] = 0;
//        second_last_line[3] = frame.rows-1;
//        direction_sign = -1;
//        max_index = min(second_best_line[0],second_best_line[2]);
//    }
//    else{
//        cout << "Right" << endl;
//        second_last_line[0] = frame.cols-1;
//        second_last_line[1] = 0;
//        second_last_line[2] = frame.cols-1;
//        second_last_line[3] = frame.rows-1;
//    }
//    int most_matches = 0;
//    int last_matches = 0;
//    for(int x_index = 0; x_index < max_index; x_index++){
//        // Get new mask
//        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
//        line(new_mask,Point(second_best_line[0]+(x_index*direction_sign),second_best_line[1]),Point(second_best_line[2]+(x_index*direction_sign),second_best_line[3]),255,3,LINE_AA);

//        // Count matches
//        Mat match_mask = new_mask & mask;
//        int match_count = countNonZero(match_mask);
//        // Check if better than previous
//        if(most_matches < match_count && first_found == false){
//            most_matches = match_count;
//            second_first_line[0] = second_best_line[0]+(x_index*direction_sign);
//            second_first_line[1] = second_best_line[1];
//            second_first_line[2] = second_best_line[2]+(x_index*direction_sign);
//            second_first_line[3] = second_best_line[3];
//            steps_since_change = 0;
//        }
//        else{
//            steps_since_change++;
//            if(steps_since_change >= 5){
//                first_found = true;
//            }
//        }
//        // Check if begining to decline
//        if(match_count < last_matches*0.75|| x_index+1 == max_index){
//            second_last_line[0] = second_best_line[0]+((x_index-1)*direction_sign);
//            second_last_line[1] = second_best_line[1];
//            second_last_line[2] = second_best_line[2]+((x_index-1)*direction_sign);
//            second_last_line[3] = second_best_line[3];
//            break;
//        }
//        last_matches = match_count;
//    }
//}
//else{
//    // horizontal edge
//    cout << "Horizontal" << endl;

//    // create directional linemasks
//    Mat up_mask = Mat::zeros(mask.size(),CV_8U);
//    Mat down_mask = Mat::zeros(mask.size(),CV_8U);
//    line(up_mask,Point(second_best_line[0],second_best_line[1]-1),Point(second_best_line[2],second_best_line[3]-1),255,3,LINE_AA); // Move one pixel up
//    line(down_mask,Point(second_best_line[0],second_best_line[1]+1),Point(second_best_line[2],second_best_line[3]+1),255,3,LINE_AA); // Move one pixel down

//    // Find direction with most matches
//    Mat up_match_mat = up_mask & mask;
//    Mat down_match_mat = down_mask & mask;
//    int up_matches = countNonZero(up_match_mat);
//    int down_matches = countNonZero(down_match_mat);

//    int direction_sign = 1;
//    int max_index = mask.rows;
//    if(up_matches > down_matches){
//        cout << "Top" << endl;
//        second_last_line[0] = 0;
//        second_last_line[1] = 0;
//        second_last_line[2] = frame.cols-1;
//        second_last_line[3] = 0;
//        direction_sign = -1;
//        max_index = min(second_best_line[1],second_best_line[3]);
//    }
//    else{
//        second_last_line[0] = 0;
//        second_last_line[1] = frame.rows-1;
//        second_last_line[2] = frame.cols-1;
//        second_last_line[3] = frame.rows-1;
//        cout << "Bottom" << endl;
//    }
//    int most_matches = 0;//max(up_matches,down_matches)
//    int last_matches = 0;
//    for(int y_index = 0; y_index < max_index; y_index++){
//        // Get new mask
//        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
//        line(new_mask,Point(second_best_line[0],second_best_line[1]+(y_index*direction_sign)),Point(second_best_line[2],second_best_line[3]+(y_index*direction_sign)),255,3,LINE_AA);

//        // Count matches
//        Mat match_mask = new_mask & mask;
//        int match_count = countNonZero(match_mask);
//        // Check if better than previous
//        if(most_matches < match_count && first_found == false){
//            most_matches = match_count;
//            second_first_line[0] = second_best_line[0];
//            second_first_line[1] = second_best_line[1]+(y_index*direction_sign);
//            second_first_line[2] = second_best_line[2];
//            second_first_line[3] = second_best_line[3]+(y_index*direction_sign);
//            steps_since_change = 0;
//        }
//        else{
//            steps_since_change++;
//            if(steps_since_change >= 5){
//                first_found = true;
//            }
//        }
//        // Check if begining to decline
//        if(match_count < last_matches*0.75 || y_index+1 == max_index){
//            second_last_line[0] = second_best_line[0];
//            second_last_line[1] = second_best_line[1]+((y_index-1)*direction_sign);
//            second_last_line[2] = second_best_line[2];
//            second_last_line[3] = second_best_line[3]+((y_index-1)*direction_sign);
//            break;
//        }
//        last_matches = match_count;
//    }
//}


//                // step 2: find best line
//                vector<Vec4i> lines;
//                HoughLinesP(edge_mask,lines,1,CV_PI/180,10,50,10);
//                int most_intersections = 0;
//                Vec4i best_line;
//                Vec4i non_extended_best_line;
//                double best_angle;
//                for(int i = 0; i < lines.size(); i++){
//                    // get points
//                    Point start = Point(lines.at(i)[0],lines.at(i)[1]);
//                    Point end = Point(lines.at(i)[2],lines.at(i)[3]);

////                    Mat baz = Mat::zeros(mask.size(),CV_8U);
////                    line(baz,start,end,255,3);
////                    resize(baz,baz,Size(),0.5,0.5,INTER_LINEAR);
////                    imshow("current line", baz);
////                    waitKey(0);

//                    // Extent lines
//                    double angle = calculations.calculate_angle(start,end);
//                    angle = angle + (90*M_PI/180); // Shift to align with x-axis
//                    double a = cos(angle);
//                    double b = sin(angle);
//                    start.x = cvRound(start.x + frame.cols*(-b));
//                    start.y = cvRound(start.y + frame.rows*(a));
//                    end.x = cvRound(end.x - frame.cols*(-b));
//                    end.y = cvRound(end.y - frame.rows*(a));

//                    // Create mask
//                    Mat line_mask = Mat::zeros(mask.size(),CV_8U);
//                    line(line_mask,start,end,255,10,LINE_AA);

//                    // Count number of intersections
//                    Mat intersection_mask = edge_mask & line_mask;
//                    int current_intersections = countNonZero(intersection_mask);

//                    // Check if bigger than previous
//                    if(current_intersections > most_intersections){
//                        most_intersections = current_intersections;
//                        non_extended_best_line = lines.at(i);
//                        lines.at(i)[0] = start.x;
//                        lines.at(i)[1] = start.y;
//                        lines.at(i)[2] = end.x;
//                        lines.at(i)[3] = end.y;

//                        best_line = lines.at(i);
//                        best_angle = angle;
//                    }
//                }


//                HoughLinesP(edge_mask,lines,1,CV_PI/180,10,50,10);

//                most_intersections = 0;
//                Vec4i second_best_line;
//                bool second_found = false;
//                Vec4i non_extended_second_best_line;
//                double second_best_angle;
//                for(int i = 0; i < lines.size(); i++){
//                    // get points
//                    Point start = Point(lines.at(i)[0],lines.at(i)[1]);
//                    Point end = Point(lines.at(i)[2],lines.at(i)[3]);

////                    Mat baz = Mat::zeros(mask.size(),CV_8U);
////                    line(baz,start,end,255,3);
////                    resize(baz,baz,Size(),0.5,0.5,INTER_LINEAR);
////                    imshow("current line", baz);
////                    waitKey(0);

//                    // Extent lines
//                    double angle = calculations.calculate_angle(start,end);
//                    angle = angle + (90*M_PI/180); // Shift to align with x-axis
//                    double a = cos(angle);
//                    double b = sin(angle);
//                    start.x = cvRound(start.x + frame.cols*(-b));
//                    start.y = cvRound(start.y + frame.rows*(a));
//                    end.x = cvRound(end.x - frame.cols*(-b));
//                    end.y = cvRound(end.y - frame.rows*(a));

//                    // Create mask
//                    Mat line_mask = Mat::zeros(mask.size(),CV_8U);
//                    line(line_mask,start,end,255,10,LINE_AA);

//                    // Count number of intersections
//                    Mat intersection_mask = edge_mask & line_mask;
//                    int current_intersections = countNonZero(intersection_mask);

//                    // Check if bigger than previous and ensure angle is not similar to best line
////                    cout << current_intersections << endl;
////                    cout << most_intersections << endl;
////                    cout << angle*180/M_PI << endl;
////                    cout << best_angle*180/M_PI << endl;
////                    cout << abs(angle-best_angle)*180/M_PI << endl;
//                    if(current_intersections > most_intersections && abs(angle-best_angle) >= (45*M_PI/180)){
//                        second_found = true;
//                        most_intersections = current_intersections;
//                        non_extended_second_best_line = lines.at(i);
//                        lines.at(i)[0] = start.x;
//                        lines.at(i)[1] = start.y;
//                        lines.at(i)[2] = end.x;
//                        lines.at(i)[3] = end.y;

//                        second_best_line = lines.at(i);
//                        second_best_angle = angle;
//                    }
//                }

//                if(second_found == false){ // Maybe change to continuing with current line just to get something
//                    cout << "could not split. Removing obstacle." << endl;
//                    break;
//                }


//                Mat viz_org = Mat::zeros(mask.size(),CV_8U);
//                line(viz_org,Point(non_extended_second_best_line[0],non_extended_second_best_line[1]),Point(non_extended_second_best_line[2],non_extended_second_best_line[3]),255,3,LINE_AA);
//                line(viz_org,Point(non_extended_best_line[0],non_extended_best_line[1]),Point(non_extended_best_line[2],non_extended_best_line[3]),255,3,LINE_AA);

//                Mat flap = viz_org.clone();
//                resize(flap,flap,Size(),0.5,0.5,INTER_LINEAR);
//                imshow("Best lines org", flap);
//                waitKey(0);

//vector<obstacle> kept_obstacles;
//for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
//    // Get current data
//    vector<Point> contour = obstacles.at(obstacle_index).contour;
//    Mat mask = obstacles.at(obstacle_index).mask;
//    // Draw bounding box
//    RotatedRect rectangle = minAreaRect(contour);
//    Point2f rectangle_points[4];
//    rectangle.points(rectangle_points);
//    vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
//    vector<vector<Point>> points;
//    points.push_back(rectangle_points_vector);
//    Mat bounding_box = Mat::zeros(mask.size(),CV_8U);
//    drawContours(bounding_box,points,0,255,-1,LINE_8);

//    Mat viz_box = mask.clone();
//    drawContours(viz_box,points,0,150,3,LINE_8);
//    resize(viz_box,viz_box,Size(),0.5,0.5,INTER_LINEAR);
//    imshow("Bounding box and mask", viz_box);
//    waitKey(0);


//    // Check if bounding box contains more than 50 percent error
//    Mat error_mask = bounding_box-mask;

//    int mask_count = countNonZero(mask);
//    int error_count = countNonZero(error_mask);

//    if(error_count > int(mask_count*0.5)){
//        // Split into two

//        // step 1: find edges in mask
//        Mat edge_mask;
//        Canny(mask,edge_mask,50,200,3);

//        Mat glop = edge_mask.clone();
//        resize(glop,glop,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("edge mask", glop);
//        waitKey(0);

//        // ---------------------------------------------- SIMPLIFICATION ZONE -------------------
//        // step 2: Find best line
//        line_data best_line_data = get_best_line(edge_mask,10,50.0,10.0,false,0.0);
//        Vec4i best_line = best_line_data.line;
//        double best_angle = best_line_data.angle;

//        // step 3: Create mask of best line
//        Mat best_line_mask = Mat::zeros(mask.size(),CV_8U);
//        line(best_line_mask,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,10,LINE_AA);

//        // step 4: remove edges under line mask
//        edge_mask = edge_mask-best_line_mask;

//        // step 5: Find new best line
//        line_data second_best_line_data = get_best_line(edge_mask,10,50.0,10.0,true,best_angle,45.0);
//        Vec4i second_best_line = second_best_line_data.line;
//        double second_best_angle = second_best_line_data.angle;

//        // step 6: Add line to mask for visualization

//        Mat viz =  Mat::zeros(mask.size(),CV_8U);
//        line(viz,Point(second_best_line[0],second_best_line[1]),Point(second_best_line[2],second_best_line[3]),255,3,LINE_AA);
//        line(viz,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,3,LINE_AA);

//        Mat flop = viz.clone();
//        resize(flop,flop,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Best lines", flop);
//        waitKey(0);

//        // Detemine direction of obstacle based on both lines
//        int direction = get_obstacle_direction(best_angle,best_line,mask);
//        int second_direction = get_obstacle_direction(second_best_angle, second_best_line,mask);

//        // Determine borders of split obstalces
//        vector<Vec4i> borders = get_line_borders(direction,best_line,mask,5,0.75);
//        Vec4i first_line = borders.at(0);
//        Vec4i last_line = borders.at(1);

//        vector<Vec4i> second_borders = get_line_borders(second_direction,second_best_line,mask,5,0.75);
//        Vec4i second_first_line = second_borders.at(0);
//        Vec4i second_last_line = second_borders.at(1);

//        // -------------------------------------------------------------------------------------------------------------------- simplification performed above ---------------------------


//        // Draw edges of best lines
//        Mat best_shape = Mat::zeros(mask.size(),CV_8U);
//        line(best_shape,Point(first_line[0],first_line[1]),Point(first_line[2],first_line[3]),255,3,LINE_AA);
//        line(best_shape,Point(last_line[0],last_line[1]),Point(last_line[2],last_line[3]),255,3,LINE_AA);

//        line(best_shape,Point(second_first_line[0],second_first_line[1]),Point(second_first_line[2],second_first_line[3]),255,3,LINE_AA);
//        line(best_shape,Point(second_last_line[0],second_last_line[1]),Point(second_last_line[2],second_last_line[3]),255,3,LINE_AA);

//        Mat mask_test = mask.clone();
//        resize(mask_test,mask_test,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("mask", mask_test);
//        waitKey(0);

//        Mat test = best_shape.clone();
//        resize(test,test,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("best shape", test);
//        waitKey(0);

//        // Fill area and view split obstacles
//        vector<Point> first_bounding;
//        first_bounding.push_back(Point(first_line[0],first_line[1]));
//        first_bounding.push_back(Point(last_line[0],last_line[1]));
//        first_bounding.push_back(Point(last_line[2],last_line[3]));
//        first_bounding.push_back(Point(first_line[2],first_line[3]));

//        Mat first_bounding_mat = Mat::zeros(mask.size(),CV_8U);
//        vector<vector<Point>> polygons;
//        polygons.push_back(first_bounding);
//        fillPoly(first_bounding_mat,polygons,255);
//        Mat first_mask = first_bounding_mat & mask;

//        vector<Point> second_bounding;
//        second_bounding.push_back(Point(second_first_line[0],second_first_line[1]));
//        second_bounding.push_back(Point(second_last_line[0],second_last_line[1]));
//        second_bounding.push_back(Point(second_last_line[2],second_last_line[3]));
//        second_bounding.push_back(Point(second_first_line[2],second_first_line[3]));

//        vector<vector<Point>> second_polygons;
//        second_polygons.push_back(second_bounding);

//        Mat second_bounding_mat = Mat::zeros(mask.size(),CV_8U);
//        fillPoly(second_bounding_mat,second_polygons,255);
//        Mat second_mask = second_bounding_mat & mask;

//        // Cut masks into only including a single obstacle
//        first_mask = ensure_single_obstacle(first_mask);
//        second_mask = ensure_single_obstacle(second_mask);

//        // Ensure remaining mask is not forgotten
//        Mat remaining_mask = mask-first_mask;
//        remaining_mask = remaining_mask-second_mask;

//        Mat mask_1 = first_mask.clone();
//        resize(mask_1,mask_1,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("first mask", mask_1);
//        waitKey(0);

//        Mat mask_2 = second_mask.clone();
//        resize(mask_2,mask_2,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("second mask", mask_2);
//        waitKey(0);

//        Mat mask_3 = remaining_mask.clone();
//        resize(mask_3,mask_3,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("remaining mask", mask_3);
//        waitKey(0);

//        // Keep if not empty
//        obstacle first_obstacle, second_obstacle, remaining_obstacle;
//        if(hasNonZero(first_mask) == true){
//            first_obstacle.mask = first_mask;
//            first_obstacle.contour = first_bounding;
//            obstacles.push_back(first_obstacle);
//        }
//        if(hasNonZero(second_mask) == true){
//            second_obstacle.mask = second_mask;
//            second_obstacle.contour = second_bounding;
//            obstacles.push_back(second_obstacle);
//        }
//        if(hasNonZero(remaining_mask) == true){
//            remaining_obstacle.mask = remaining_mask;
//            remaining_obstacle.original_mask = mask;
//            obstacles.push_back(remaining_obstacle);
//        }
//    }
//    else{
//        // Keep obstacle if box is good fit
//        obstacles.at(obstacle_index).type = "Unknown";
//        kept_obstacles.push_back(obstacles.at(obstacle_index));
//    }
//}
//final_obstacles = kept_obstacles;

// Get all valid points
//vector<float> depths;
//for(int row_index = 0; row_index < depth_channel.rows; row_index++){
//    for(int col_index = 0; col_index < depth_channel.cols; col_index++){
//        float depth = depth_channel.at<float>(Point(col_index,row_index));
//        if(isinf(depth) == false && isinf(depth) == false && isinf(depth) == false && isnan(depth) == false && isnan(depth) == false && isnan(depth) == false){
//            depths.push_back(depth);
//        }
//    }
//}


//Mat depth_copy = depth_channel.clone();
//depth_channel = org_depth.clone();

//// Timing
//start = chrono::high_resolution_clock::now();
//int baddie_count = 0;
//for(int row_index = 0; row_index < depth_channel.rows; row_index++){
//    for(int col_index = 0; col_index < depth_channel.cols; col_index++){
//        // Get depth / z-coordinate
//        float depth = depth_channel.at<float>(Point(col_index,row_index));

//        // Set to average of soroundings if Nan or inf or bigger than percentile
//        if(isinf(depth) == true || isinf(depth) == true || isinf(depth) == true || isnan(depth) == true || isnan(depth) == true || isnan(depth) == true){
//            baddie_count++;
//            float area_sum = 0.0;
//            int values = 0;

//            int min_row = max(row_index,depth_channel.rows-row_index);
//            int min_col = max(col_index, depth_channel.cols-col_index);

//            int max_ring = max(min_row,min_col);
//            bool found = false;
//            int start_row,end_row,start_col,end_col;
//            for(int ring = 1; ring < max_ring; ring++){
//                start_row = max(0,row_index-ring);
//                end_row = min(depth_channel.rows-1,row_index+ring);

//                start_col = max(0,col_index-ring);
//                end_col = min(depth_channel.cols-1,col_index+ring);

//                for(int current_row = start_row; current_row <= end_row; current_row++){
//                    for(int current_col = start_col; current_col <= end_col; current_col++){
//                        // get depth
//                        Vec3f channel_data = depth_map.at<Vec3f>(Point(current_col,current_row));
//                        float kernel_depth = channel_data[2]; // Important to take from initial depth map, since depth channel is manipulated during runtime.
//                        // check if valid
//                        if(isinf(kernel_depth) == false && isinf(kernel_depth) == false && isinf(kernel_depth) == false && isnan(kernel_depth) == false && isnan(kernel_depth) == false && isnan(kernel_depth) == false){
//                            area_sum += kernel_depth;
//                            values++;
//                            found = true;
//                        }
//                    }
//                }
//                if(found == true){
//                    break;
//                }
//            }


//            // if no neighbors found, set to max. otherwise set to average
//            if(area_sum == 0.0){
//                depth = 0.0;
//            }
//            else{
//                depth = area_sum/values;
//                //cout << "old: " << depth << endl;
//                //cout << "new: " << depth_copy.at<float>(Point(col_index,row_index)) << endl;
//            }
//            if(baddie_count < 10){
//                cout << start_col << ", " << start_row << " -> " << end_col << ", " << end_row << " | " << depth << endl;
//            }

//        }

//        // If bigger than max
//        if(depth > max_depth){
//            depth = max_depth;
//        }
//        // If smaller set to min
//        if(depth < min_depth){
//            depth = min_depth;
//        }

//        // Normalize
//        float normalized_depth = (depth-min_depth)/(max_depth-min_depth)*range;

//        // Assign depth
//        depth_channel.at<float>(Point(col_index,row_index)) = normalized_depth;
//    }
//}
//stop = chrono::high_resolution_clock::now();
//duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//cout << "Old solution done in  " << duration.count() << " ms using." << endl;

//        bool bottom_found = false;
//        bool top_found = false;
//        int low_index,high_index;
//        for(int indent = 0; indent < sorted_data_points.size(); indent++){
//            float lower_val = sorted_data_points.at(indent);
//            float upper_val = sorted_data_points.at(sorted_data_points.size()-(indent+1));

//            if(lower_val >= lower_bound && bottom_found == false){
//                low_index = indent;
//                bottom_found = true;
//            }
//            if(upper_val <= upper_bound && top_found == false){
//                high_index = indent;
//                top_found = true;
//            }
//            if(top_found == true && bottom_found == true){
//                break;
//            }
//        }

//        filtered_data.assign(sorted_data_points.begin()+low_index,sorted_data_points.end()-high_index);

// Ensure that we only work in the area of interest
//            if(frame_index < 6950){
//                continue;
//            }
//            if(frame_index > 7470){
//                break;
//            }

//        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end();){
//            int current_size = it->size();

//            if(current_size > disparity_map.cols*disparity_map.rows*0.01){
//                big_contours.push_back(*it);
//            }
//            else{
//                ++it;
//            }
//        }

//        // Create mask for invalid values
//        Mat invalid = disparity_map == -16;

//        // Dilate mask one pixel in rows alone to left and to right
//        Mat_<int> temp_left_kernel(3,3);
//        temp_left_kernel << 0,0,0,0,255,255,0,0,0;
//        Mat left_kernel = temp_left_kernel;
//        left_kernel.convertTo(left_kernel,CV_8U);
//        Mat left_valid;
//        dilate(invalid,left_valid,left_kernel);

//        Mat_<int> temp_right_kernel(3,3);
//        temp_right_kernel << 0,0,0,255,255,0,0,0,0;
//        Mat right_kernel = temp_right_kernel;
//        right_kernel.convertTo(right_kernel,CV_8U);
//        Mat right_valid;
//        dilate(invalid,right_valid,right_kernel);

////        // Remove invalid mask from valid mask to get closest pixels
//        left_valid = left_valid - invalid;
//        right_valid = right_valid - invalid;

//        // Get values of valid areas
//        imshow("invalid", invalid);
//        imshow("left",left_valid);
//        imshow("right", right_valid);
//        waitKey(0);

        // Get coordinates of invalid points as well as closest valid points
//        vector<Point> locations;
//        findNonZero(invalid,locations);

//        vector<Point> valid_locations;
//        findNonZero(valid,valid_locations);

//        // For each location find closest valid left and right location
//        for(int i = 0; i < locations.size(); i++){

//        }


//vector<obstacle> detecting::split_into_rectangles_corner(vector<obstacle> obstacles){
//    vector<obstacle> accepted_obstacles;
//    try{
//        // Step 0: Split current obstacles into rectangles
//        vector<Mat> viable_splits;
//        vector<Mat> original_masks;

//        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){

//            // Get current data
//            vector<Point> contour = obstacles.at(obstacle_index).contour;
//            Mat mask = obstacles.at(obstacle_index).mask;

//            // Draw bounding rectangle
//            RotatedRect current_rectangle = minAreaRect(contour);
//            Point2f rectangle_points[4];
//            current_rectangle.points(rectangle_points);
//            vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
//            vector<vector<Point>> points;
//            points.push_back(rectangle_points_vector);
//            Mat bounding_box = Mat::zeros(mask.size(),CV_8U);
//            drawContours(bounding_box,points,0,WHITE,-1,LINE_8);

//            // Check if bounding box contains more than threshold percent error (Error being pixel within box that do not belong to mask)
//            Mat error_mask = bounding_box-mask;
//            int mask_count = countNonZero(mask);
//            int error_count = countNonZero(error_mask);

//            // If big error the obstacle should be split
//            if(error_count > int(mask_count*accept_rectangle_threshold)){
//                vector<Mat> obstacles_to_be_split = {mask};
//                int original_contour_size = mask_count;

//                while(obstacles_to_be_split.size() > 0){

//                    // Step 1: find biggest remaining contour
//                    vector<vector<Point>> contours;
//                    vector<Vec4i> hierarchy;
//                    findContours(obstacles_to_be_split.at(0),contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//                    int biggest = 0;
//                    vector<Point> biggest_contour;
//                    for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
//                        int current_size = it->size();

//                        if(current_size > biggest){
//                            biggest = current_size;
//                            biggest_contour = *it;
//                        }
//                        else{
//                            ++it;
//                        }
//                    }
//                    vector<vector<Point>> temp_contours = {biggest_contour};
//                    Mat contour_mask =  Mat::zeros(mask.size(),CV_8U);
//                    drawContours(contour_mask,temp_contours,0,WHITE,-1);

//                    // Step 2: Find edges
//                    Mat edge_mask;
//                    Canny(contour_mask,edge_mask,50,200,3);


//                    Mat test = Mat::zeros(mask.size(),CV_8U);
//                    drawContours(test,temp_contours,0,WHITE,1);
//                    imshow("the mask", contour_mask);
//                    imshow("the edge", edge_mask);
//                    imshow("new idea", test);
//                    waitKey(0);

//                    // Step 3: Find best but different lines
//                    line_data best_line_data, second_best_line_data;
//                    best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,false,false);
//                    Vec4i best_line = best_line_data.line;
//                    double best_angle = best_line_data.angle;
//                    second_best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,true,best_angle, 45.0);
//                    Vec4i second_best_line = second_best_line_data.line;
//                    double second_best_angle = second_best_line_data.angle;

//                    // Stop if two different lines cant be found
//                    if(second_best_line[0] == second_best_line[2] && second_best_line[1] == second_best_line[3]){
//                        break;
//                    }

//                    // Step 4: Move lines until obstacle is lost
//                    int first_direction = get_obstacle_direction(best_angle,best_line,contour_mask);
//                    int second_direction = get_obstacle_direction(second_best_angle,second_best_line,contour_mask);

//                    vector<Vec4i> first_movement = get_line_borders(first_direction,best_line,contour_mask,step_threshold,decline_threshold);
//                    Vec4i new_best_line = first_movement.at(1);

//                    vector<Vec4i> second_movement = get_line_borders(second_direction,second_best_line,contour_mask,step_threshold,decline_threshold);
//                    Vec4i new_second_best_line = second_movement.at(1);

//                    // Step 5: Detect inner corner (The corner where cutting with either line will result in there being an obstacle in both cuts)
//                    bool best_line_validity = check_valid_split(best_line,first_direction,mask);
//                    bool second_best_line_validity = check_valid_split(second_best_line, second_direction,mask);
//                    bool new_best_validity = check_valid_split(new_best_line,first_direction, mask);
//                    bool new_second_best_validity = check_valid_split(new_second_best_line, second_direction, mask);

//                    vector<Vec4i> split_lines;
//                    if(best_line_validity == true && second_best_line_validity == true){
//                        split_lines = {best_line,second_best_line};
//                    }
//                    else if(new_best_validity == true && new_second_best_validity == true){
//                        split_lines = {new_best_line, new_second_best_line};
//                    }
//                    else{
//                        throw runtime_error("No inner corner could be found.");
//                    }


//                    // Step 6: split obstacle in two based on lines and direction
//                    Mat best_split = get_best_split(split_lines.at(0),first_direction,mask);
//                    Mat second_best_split = get_best_split(split_lines.at(1),second_direction,mask);

//                    // Get best split masks
//                    Mat best_split_mask = get_best_split_mask(split_lines.at(0),first_direction,mask);
//                    Mat second_best_split_mask = get_best_split_mask(split_lines.at(1),second_direction,mask);

//                    // Get point of no intersection
//                    Mat no_go_zone = best_split_mask | second_best_split_mask;
//                    bitwise_not(no_go_zone,no_go_zone);

//                    // Remove this areas for best splits
//                    Mat no_go_zone_best = no_go_zone & best_split;
//                    bitwise_xor(best_split,no_go_zone_best,best_split);
//                    Mat no_go_zone_second_best = no_go_zone & second_best_split;
//                    bitwise_xor(second_best_split,no_go_zone_second_best,second_best_split);

//                    // Step 7: Check if cuts can be estimated as rectangles
//                    vector<vector<Point>> best_split_contours;
//                    vector<Vec4i> best_split_hierarchy;
//                    findContours(best_split,best_split_contours,best_split_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//                    int best_split_biggest = 0;
//                    vector<Point> best_split_biggest_contour;
//                    for(vector<vector<Point>>::iterator it = best_split_contours.begin(); it!=best_split_contours.end(); ){
//                        int current_size = it->size();

//                        if(current_size > best_split_biggest){
//                            best_split_biggest = current_size;
//                            best_split_biggest_contour = *it;
//                        }
//                        else{
//                            ++it;
//                        }
//                    }

//                    Mat best_mask = Mat::zeros(mask.size(),CV_8U);
//                    vector<vector<Point>> temp = {best_split_biggest_contour};
//                    drawContours(best_mask,temp,0,255,-1);

//                    vector<vector<Point>> second_best_split_contours;
//                    vector<Vec4i> second_best_split_hierarchy;
//                    findContours(second_best_split,second_best_split_contours,second_best_split_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//                    int second_best_split_biggest = 0;
//                    vector<Point> second_best_split_biggest_contour;
//                    for(vector<vector<Point>>::iterator it = second_best_split_contours.begin(); it!=second_best_split_contours.end(); ){
//                        int current_size = it->size();

//                        if(current_size > second_best_split_biggest){
//                            second_best_split_biggest = current_size;
//                            second_best_split_biggest_contour = *it;
//                        }
//                        else{
//                            ++it;
//                        }
//                    }

//                    Mat second_best_mask = Mat::zeros(mask.size(),CV_8U);
//                    vector<vector<Point>> second_temp = {second_best_split_biggest_contour};
//                    drawContours(second_best_mask,second_temp,0,255,-1);

//                    RotatedRect best_rectangle = minAreaRect(best_split_biggest_contour);
//                    Point2f best_rectangle_points[4];
//                    best_rectangle.points(best_rectangle_points);
//                    vector<Point> best_rectangle_points_vector = {best_rectangle_points[0],best_rectangle_points[1],best_rectangle_points[2],best_rectangle_points[3]};
//                    vector<vector<Point>> best_points;
//                    best_points.push_back(best_rectangle_points_vector);
//                    Mat best_bounding_box = Mat::zeros(mask.size(),CV_8U);
//                    drawContours(best_bounding_box,best_points,0,WHITE,-1,LINE_8);

//                    RotatedRect second_best_rectangle = minAreaRect(second_best_split_biggest_contour);
//                    Point2f second_best_rectangle_points[4];
//                    second_best_rectangle.points(second_best_rectangle_points);
//                    vector<Point> second_best_rectangle_points_vector = {second_best_rectangle_points[0],second_best_rectangle_points[1],second_best_rectangle_points[2],second_best_rectangle_points[3]};
//                    vector<vector<Point>> second_best_points;
//                    second_best_points.push_back(second_best_rectangle_points_vector);
//                    Mat second_best_bounding_box = Mat::zeros(mask.size(),CV_8U);
//                    drawContours(second_best_bounding_box,second_best_points,0,WHITE,-1,LINE_8);

//                    Mat best_error_mask = best_bounding_box-best_mask;
//                    int best_mask_count = countNonZero(best_mask);
//                    int best_error_count = countNonZero(best_error_mask);

//                    Mat second_best_error_mask = second_best_bounding_box-second_best_mask;
//                    int second_best_mask_count = countNonZero(second_best_mask);
//                    int second_best_error_count = countNonZero(second_best_error_mask);

//                    // What about remaining data cut out during splitting?
//                    if(best_error_count > int(best_mask_count*accept_rectangle_threshold) && best_mask_count > original_contour_size*obstacle_size_limit){
//                        obstacles_to_be_split.push_back(best_split);
//                    }
//                    else if(best_mask_count > original_contour_size*obstacle_size_limit){
//                        viable_splits.push_back(best_mask);
//                        original_masks.push_back(mask);
//                    }
//                    if(second_best_error_count > int(second_best_mask_count*accept_rectangle_threshold) && second_best_mask_count > original_contour_size*obstacle_size_limit){
//                        obstacles_to_be_split.push_back(second_best_split);
//                    }
//                    else if(second_best_mask_count > original_contour_size*obstacle_size_limit){
//                        viable_splits.push_back(second_best_mask);
//                        original_masks.push_back(mask);
//                    }
//                    Mat remaining_mask = obstacles_to_be_split.at(0)-best_mask;
//                    remaining_mask = remaining_mask - second_best_mask;

//                    if(countNonZero(remaining_mask) > original_contour_size*obstacle_size_limit){
//                        obstacles_to_be_split.push_back(remaining_mask);
//                    }

//                   obstacles_to_be_split.erase(obstacles_to_be_split.begin());
//                }
//            }
//            else{
//                viable_splits.push_back(mask);
//                original_masks.push_back(mask);
//            }
//        }

//        // Clean viable obstacles
//        for(int i = 0; i < viable_splits.size(); i++){
//            // Find contour
//            vector<Point> contour;
//            findNonZero(viable_splits.at(i),contour);

//            // Find best fit line for each obstacle
//            Vec4f fitted_line;
//            fitLine(contour,fitted_line, DIST_L2, 0, 0.1, 0.1);

//            // Find angle of line
//            int biggest_distance = int(sqrt(viable_splits.at(i).cols*viable_splits.at(i).cols + viable_splits.at(i).rows * viable_splits.at(i).rows));
//            Point start = Point(fitted_line[2]-fitted_line[0]*biggest_distance,fitted_line[3]-fitted_line[1]*biggest_distance);
//            Point end = Point(fitted_line[2]+fitted_line[0]*biggest_distance,fitted_line[3]+fitted_line[1]*biggest_distance);

//            double angle = calculations.calculate_angle(start,end);
//            angle = angle + (90*M_PI/180); // Shift to align with x-axis

//            // Check if line is vertical or horizontal
//            int direction;
//            if(angle*180/M_PI < abs(angle*180/M_PI-90)){
//                direction = DIRECTION_RIGHT;
//            }
//            else{
//                direction = DIRECTION_UP;
//            }

//            // Get best fit mask
//            Vec4i current_line;
//            current_line[0] = start.x;
//            current_line[1] = start.y;
//            current_line[2] = end.x;
//            current_line[3] = end.y;
//            vector<Vec4i> borders = get_best_fit_borders(direction,current_line,viable_splits.at(i));

//            Mat best_fit_mask = Mat::zeros(viable_splits.at(i).size(),CV_8U);
//            vector<Point> points = {Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]), Point(borders.at(0)[2],borders.at(0)[3])};
//            vector<vector<Point>> temp = {points};
//            drawContours(best_fit_mask,temp,0,255,-1);

//            Mat final_mask = best_fit_mask & viable_splits.at(i);


//            obstacle current_obstacle;
//            current_obstacle.type = "Unknown";

//            vector<vector<Point>> contours;
//            vector<Vec4i> hierarchy;
//            findContours(final_mask,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//            int biggest = 0;
//            vector<Point> biggest_contour;
//            for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
//                int current_size = it->size();

//                if(current_size > biggest){
//                    biggest = current_size;
//                    biggest_contour = *it;
//                }
//                else{
//                    ++it;
//                }
//            }
//            current_obstacle.contour = biggest_contour;

//            // use rectangle as obstacle shape
//            RotatedRect current_rectangle = minAreaRect(biggest_contour);
//            Point2f rectangle_points[4];
//            current_rectangle.points(rectangle_points);
//            vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
//            vector<vector<Point>> temp_points;
//            temp_points.push_back(rectangle_points_vector);
//            Mat bounding_box = Mat::zeros(viable_splits.at(i).size(),CV_8U);
//            drawContours(bounding_box,temp_points,0,WHITE,-1,LINE_8);

//            current_obstacle.mask = bounding_box;
//            current_obstacle.original_mask = original_masks.at(i);

//            accepted_obstacles.push_back(current_obstacle);
//        }
//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return accepted_obstacles;
//}

//                            // Check if identical to previous mask
//                            Mat difference;
//                            bitwise_xor(new_contour_mask,temp_obstacles.at(k).mask,difference);

//                            if(countNonZero(difference) == 0){
////                                imshow("the killed",new_contour_mask);
////                                imshow("the killer",temp_obstacles.at(k).mask);
////                                waitKey(0);
//                                accept = false;
//                                break;
//                            }


//                // Remove all obstacles wholy within others
//                for(int i = 0; i < temp_obstacles.size(); i++){
//                    bool within_another = false;
////                    imshow("obstacle",temp_obstacles.at(i).mask);
//                    // new test for overlap
////                    Mat remainder = temp_obstacles.at(i).mask.clone();
//                    for(int j = 0; j < temp_obstacles.size(); j++){
//                        if(i != j){
//                            // new test (remove overlap as we go)
////                            remainder = remainder-temp_obstacles.at(j).mask;
//                            //bitwise_xor(not_overlap,temp_obstacles.at(j).mask,not_overlap);
//                            //Mat temp = not_overlap & temp_obstacles.at(j).mask;
//                            //not_overlap = not_overlap - temp;

////                            imshow("overlap",remainder);
////                            imshow("new obstacle",temp_obstacles.at(j).mask);
////                            waitKey(0);

////                            if(countNonZero(remainder) == 0){
////                                break;
////                            }
//                            // Just remove obstacles wholly within another
//                            Mat overlap = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
//                            int overlap_count = countNonZero(overlap);

//                            if(overlap_count == countNonZero(temp_obstacles.at(i).mask)){
//                                within_another = true;
//                            }

//                            // Test for overlap while ensuring that they are not just the same
////                            Mat overlap = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
////                            int overlap_count = countNonZero(overlap);

////                            if(overlap_count == countNonZero(temp_obstacles.at(i).mask) && countNonZero(temp_obstacles.at(i).mask) != countNonZero(temp_obstacles.at(j).mask)){
////                                within_another = true;
////                            }
////                            // Combine obstacles that are very similar
////                            else if(overlap_count >= int(countNonZero(temp_obstacles.at(i).mask)*overlap_threshold)){
////                                temp_obstacles.at(i).mask = temp_obstacles.at(i).mask | temp_obstacles.at(j).mask;
////                            }
//                        }
//                    }
//                    // New check (check for some unique part)
////                    string survived = "killed";
////                    if(countNonZero(remainder) > 0){
////                        accepted_obstacles.push_back(temp_obstacles.at(i));
////                        survived = "survived";
////                    }

//                    string survived = "killed";
//                    if(within_another == false){
//                        accepted_obstacles.push_back(temp_obstacles.at(i));
//                        survived = "survived";
//                    }
////                    cout << survived << endl;
////                    waitKey(0);
//                }

//            cout << accepted_obstacles.size() << endl;


//                    Mat new_intersection = new_version & full_other_map;
//                    int new_intersection_count = countNonZero(new_intersection);
//                    if(new_intersection_count < countNonZero(new_version)*0.95){
//                        temp_obstacles.at(i).mask = new_version;
//                        accepted_obstacles.push_back(temp_obstacles.at(i));
//                    }
//                    else{
//                        imshow("oh no",new_version);
//                        imshow("killer",full_other_map);
//                        waitKey(0);
//                    }
//                        else{
//                            imshow("not sim",temp_obstacles.at(j).mask);
//                            waitKey(0);
////                            full_other_map = full_other_map | temp_obstacles.at(j).mask;
//                        }


//                    cout << "remaining: " << temp_obstacles.size() << endl;
//                    imshow("obs",temp_obstacles.at(i).mask);
//                    waitKey(0);
//                    cout << "remove: " << endl;
//                    Mat full_other_map = Mat::zeros(new_version.size(),CV_8U);
//cout << "size: " << frame_size << endl;

//Mat test_1 = Mat::zeros(frame_size,CV_8U);

//for(int i = 0; i < contour.size(); i++){
//    test_1.at<uchar>(contour.at(i)) = 255;
//}

//dilate(test_1,test_1,border_kernel,Point(-1,-1),10);

////        vector<vector<Point>> temp = {contour};
////        drawContours(test_1,temp,0,WHITE,DRAW_WIDTH_INFILL);
////        imshow("input contour",test_1);

//// test flip and stuff

//// find edge to add to
//Mat left = Mat::zeros(frame_size, CV_8U);
//Mat right = Mat::zeros(frame_size, CV_8U);
//Mat top = Mat::zeros(frame_size, CV_8U);
//Mat bottom = Mat::zeros(frame_size, CV_8U);

//line(left,Point(0,0),Point(0,frame_size.height-1),WHITE);
//line(right,Point(frame_size.width-1,0),Point(frame_size.width-1,frame_size.height-1),WHITE);
//line(bottom, Point(0,frame_size.height-1),Point(frame_size.width-1,frame_size.height-1),WHITE);
//line(top, Point(0,0), Point(frame_size.width-1,0),WHITE);

//int count_left, count_right, count_top, count_bottom;
//count_left = countNonZero(left & test_1);
//count_right = countNonZero(right & test_1);
//count_top = countNonZero(top & test_1);
//count_bottom = countNonZero(bottom & test_1);

////        cout << count_left << ", " << count_right << ", " << count_top << ", " << count_bottom << endl;
//Size draw_size = frame_size;
//int start_x = 0;
//int start_y = 0;
//int end_x = frame_size.width;
//int end_y = frame_size.height;

//int number_edges = 0;
//if(count_left > 0){
//    number_edges++;
//}
//if(count_right > 0){
//    number_edges++;
//}
//if(count_top > 0){
//    number_edges++;
//}
//if(count_bottom > 0){
//    number_edges++;
//}
//Mat test_2;
//vector<Point> big_contour;
//if(number_edges == 0){
//    big_contour = contour;
//}
//else{
//    Mat comb;

//    Mat testflip = test_1.clone();
//    Mat testflip2 = test_1.clone();
//    flip(testflip,testflip,1);
//    flip(testflip,testflip,0);
//    flip(testflip2,testflip2,0);
//    flip(testflip2,testflip2,1);
//    imshow("flip 1 ", testflip);
//    imshow("flip 2", testflip2);
//    waitKey(0);
//    if(max(count_left,count_right) > max(count_top,count_bottom)){
//        flip(test_1,test_2,1);
//        flip(test_2,test_2,0);
//        if(number_edges == 1){
//            Point center_1 = get_contour_center(test_1);
//            Point center_2 = get_contour_center(test_2);

//            int dist =  center_1.y-center_2.y;

//            Mat full = (left & test_1) | (right & test_1) | (bottom & test_1) | (top & test_1);

//            vector<Point> locations;
//            findNonZero(full,locations);

//            int min_y = test_1.rows;
//            int max_y = 0;

//            for(int k = 0; k < locations.size();k++){
//                if(locations.at(k).y < min_y){
//                    min_y = locations.at(k).y;
//                }
//                if(locations.at(k).y > max_y){
//                    max_y = locations.at(k).y;
//                }
//            }

//            dist += abs(max_y-center_1.y)-abs(min_y-center_1.y);

//            // Shift
//            float translation_array[] = {1.0,0.0,0.0,0.0,1.0,float(dist)};
//            Mat translation = Mat(2,3,CV_32F,translation_array);

//            warpAffine(test_2,test_2,translation,test_2.size());
//        }

//        if(count_left > count_right && count_left > count_top && count_left > count_bottom){
//            start_x = frame_size.width;
//            end_x = frame_size.width*2;
//            hconcat(test_2,test_1,comb);
//        }
//        else{
//            hconcat(test_1,test_2,comb);
//        }

//        // add more to top to ensure long enough shape
//        if(number_edges > 2){
//            Mat top_edge = top & test_1;
//            Mat bottom_edge = bottom & test_1;

//            vector<Point> top_locations, bottom_locations;
//            findNonZero(top_edge,top_locations);
//            findNonZero(bottom_edge,bottom_locations);

//            int top_biggest_x = 0;
//            int bottom_biggest_x = 0;
//            int top_smallest_x = test_1.cols;
//            int bottom_smallest_x = test_1.cols;

//            for(int k = 0; k < top_locations.size(); k++){
//                if(top_locations.at(k).x > top_biggest_x){
//                    top_biggest_x = top_locations.at(k).x;
//                }
//                if(top_locations.at(k).x < top_smallest_x){
//                    top_smallest_x = top_locations.at(k).x;
//                }
//            }

//            for(int k = 0; k < bottom_locations.size(); k++){
//                if(bottom_locations.at(k).x > bottom_biggest_x){
//                    bottom_biggest_x = bottom_locations.at(k).x;
//                }
//                if(bottom_locations.at(k).x < bottom_smallest_x){
//                    bottom_smallest_x = bottom_locations.at(k).x;
//                }
//            }


//            float diff;
//            if(abs(float(top_biggest_x-bottom_biggest_x)) > abs(float(top_smallest_x-bottom_smallest_x))){
//                diff = float(top_biggest_x-bottom_biggest_x);
//            }
//            else{
//                diff = float(top_smallest_x-bottom_smallest_x);
//            }

//            float translation_array[] = {1.0,0.0,diff,0.0,1.0,0.0};
//            Mat translation = Mat(2,3,CV_32F,translation_array);

//            Mat temp_comb;
//            warpAffine(comb,temp_comb,translation,comb.size());
//            start_y = frame_size.height;
//            end_y = frame_size.height*2;
//            vconcat(temp_comb,comb,comb);
//        }
//    }
//    else{
//        flip(test_1,test_2,0);
//        flip(test_2,test_2,1);

//        if(number_edges == 1){
//            Point center_1 = get_contour_center(test_1);
//            Point center_2 = get_contour_center(test_2);

//            int dist =  center_1.x-center_2.x;

//            Mat full = (left & test_1) | (right & test_1) | (bottom & test_1) | (top & test_1);

//            vector<Point> locations;
//            findNonZero(full,locations);

//            int min_x = test_1.cols;
//            int max_x = 0;

//            for(int k = 0; k < locations.size();k++){
//                if(locations.at(k).x < min_x){
//                    min_x = locations.at(k).x;
//                }
//                if(locations.at(k).x > max_x){
//                    max_x = locations.at(k).x;
//                }
//            }

//            dist += abs(max_x-center_1.x)-abs(min_x-center_1.x);

//            // Shift
//            float translation_array[] = {1.0,0.0,float(dist),0.0,1.0,0.0};
//            Mat translation = Mat(2,3,CV_32F,translation_array);

//            warpAffine(test_2,test_2,translation,test_2.size());
//        }

//        if(count_top > count_right && count_top > count_left && count_top > count_bottom){
//            start_y = frame_size.height;
//            end_y = frame_size.height*2;
//            vconcat(test_2,test_1,comb);
//        }
//        else{
//            vconcat(test_1,test_2,comb);
//        }
//        // add more to left to ensure long enough shape
//        if(number_edges > 2){
//            Mat left_edge = left & test_1;
//            Mat right_edge = right & test_1;
//            vector<Point> left_locations, right_locations;
//            findNonZero(left_edge,left_locations);
//            findNonZero(right_edge,right_locations);

//            int left_biggest_y = 0;
//            int right_biggest_y = 0;
//            int left_smallest_y = test_1.rows;
//            int right_smallest_y = test_1.rows;

//            for(int k = 0; k < left_locations.size(); k++){
//                if(left_locations.at(k).y > left_biggest_y){
//                    left_biggest_y = left_locations.at(k).y;
//                }
//                if(left_locations.at(k).y < left_smallest_y){
//                    left_smallest_y = left_locations.at(k).y;
//                }

//            }

//            for(int k = 0; k < right_locations.size(); k++){
//                if(right_locations.at(k).y > right_biggest_y){
//                    right_biggest_y = right_locations.at(k).y;
//                }
//                if(right_locations.at(k).y < right_smallest_y){
//                    right_smallest_y = right_locations.at(k).y;
//                }
//            }

//            float diff;
//            if(abs(float(left_biggest_y-right_biggest_y)) > abs(float(left_smallest_y-right_smallest_y))){
//                diff = float(left_biggest_y-right_biggest_y);
//            }
//            else{
//                diff = float(left_smallest_y-right_smallest_y);
//            }

//            float translation_array[] = {1.0,0.0,0.0,0.0,1.0,diff};
//            Mat translation = Mat(2,3,CV_32F,translation_array);

//            Mat temp_comb;
//            warpAffine(comb,temp_comb,translation,comb.size());
//            start_x = frame_size.width;
//            end_x = frame_size.width*2;
//            hconcat(temp_comb,comb,comb);
//        }
//    }
//    erode(comb,comb,border_kernel,Point(-1,-1),10);
//    draw_size = comb.size();
//    big_contour = get_biggest_contour(comb);
////            Mat comb_temp;
////            resize(comb,comb_temp,Size(),0.5,0.5,INTER_LINEAR);

////            imshow("feaf",comb_temp);
////            waitKey(0);
///
//}

// If there is an intersection gather the points of second polygon
//vector<Point> second_points = temp_triangles.at(j).points;
//// Find points in first polygon that is on intersection
//vector<Point> first_intersections = {};
//for(int k = 0; k < first_points.size(); k++){
//    if(intersection.at<uchar>(first_points.at(k)) == uchar(WHITE)){
//        first_intersections.push_back(first_points.at(k));
//    }
//}
//// Find points in second polygon that is on intersection
//vector<Point> second_intersections = {};
//for(int k = 0; k < second_points.size(); k++){
//    if(intersection.at<uchar>(second_points.at(k)) == uchar(WHITE)){
//        second_intersections
//    }
//}


//// Find points in second polygon to be added
//vector<Point> points_to_add = {};
//int current_common_index = 0;
//bool finding_unique = false;
//int gaps = 0;

//for(int s = common_indexes_in_second.at(0); s < second_points.size(); s++){ // from first connection point to end
//    if(s != common_indexes.at(current_common_index)){
//        if(finding_unique == false){
//            gaps++;
//        }
//        points_to_add.push_back(second_points.at(s));
//        finding_unique = true;
//    }
//    else{
//        current_common_index++;
//        finding_unique = false;
//    }
//}

//for(int s = 0; s < common_indexes_in_second.at(0); s++){ // from start to first connection point
//    if(s != common_indexes.at(current_common_index)){
//        if(finding_unique == false){
//            gaps++;
//        }
//        finding_unique = true;
//        points_to_add.push_back(second_points.at(s));
//    }
//    else{
//        current_common_index++;
//        finding_unique = false;
//    }
//}

//// If gaps exist continue to next polygon since these gaps present incomplete connections
//if(gaps > 1){
//    continue;
//}

//                    vector<Point> combined_points = {};
//                    int return_index = 0;
//                    for(int f = 0; f < first_points.size(); f++){
//                        combined_points.push_back(first_points.at(f));
//                        if(first_points.at(f) == return_point){
//                            return_index = f+1;
//                            break;
//                        }
//                    }


//                    // Add remaining of first polygon
//                    for(int f = return_index; f < first_points.size(); f++){
//                        combined_points.push_back(first_points.at(f));
//                    }

// Draw line from the last new point to point of connection (used to check for possible different point of connection)
//                    Point last_new_point = points_to_add.back();

//                    if(common_indexes_in_second.back() == first_points.size()-1){ // If last conection point is the last overall point, the line actually goes from that to the begining, so the connection point should be later
//                        point_of_connection = common_elements.at(common_elements.size()-2);
//                    }

//                    Mat connection_line = Mat::zeros(first_mask.size(),CV_8U);
//                    line(connection_line,last_new_point,point_of_connection,WHITE,DRAW_WIDTH_1P);

//                    // Check for intersection with all other lines
//                    current_common_index = 0;
//                    vector<Point> possible_connection_points = {};
//                    vector<int> possible_connection_indexes = {};
//                    for(int  f = 0; f < first_points.size(); f++){
//                        if(f != common_indexes.at(current_common_index)){
//                            Mat current_line = Mat::zeros(first_mask.size(),CV_8U);
//                            int second_point_index = f+1;
//                            if(f != first_points.size()-1){
//                                line(current_line,first_points.at(f),first_points.at(f+1),WHITE,DRAW_WIDTH_1P);
//                            }
//                            else{
//                                line(current_line,first_points.at(f),first_points.at(0),WHITE,DRAW_WIDTH_1P);
//                                second_point_index = 0;
//                            }
//                            // Check for intersection that is not due to being connected to same point
//                            if(hasNonZero(current_line & connection_line) && point_of_connection != first_points.at(second_point_index)){
//                                possible_connection_points.push_back(first_points.at(f));
//                                possible_connection_indexes.push_back(f);
//                            }
//                        }
//                        else{
//                            if(current_common_index + 1 < first_points.size()){
//                                current_common_index++;
//                            }
//                        }
//                    }






//                    // Find start and end of connection for first points
//                    int first_start = 0;
//                    int first_end = 0;
//                    for(int f = 0; f < first_points.size(); f++){
//                        int previous_index = f-1;
//                        int next_index = f+1;
//                        if(previous_index < 0){
//                            previous_index = first_points.size()-1;
//                        }
//                        if(next_index >= first_points.size()){
//                            next_index = 0;
//                        }

//                        if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(first_points.at(previous_index)) != uchar(WHITE)){
//                            first_start = f;
//                        }
//                        if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(first_points.at(next_index)) != uchar(WHITE)){
//                            first_end = f;
//                        }
//                    }

//                    // do the same for the second points
//                    int second_start = 0;
//                    int second_end = 0;
//                    for(int f = 0; f < second_points.size(); f++){
//                        int previous_index = f-1;
//                        int next_index = f+1;
//                        if(previous_index < 0){
//                            previous_index = second_points.size()-1;
//                        }
//                        if(next_index >= second_points.size()){
//                            next_index = 0;
//                        }

//                        if(intersection.at<uchar>(second_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(second_points.at(previous_index)) != uchar(WHITE)){
//                            second_start = f;
//                        }
//                        if(intersection.at<uchar>(second_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(second_points.at(next_index)) != uchar(WHITE)){
//                            second_end = f;
//                        }
//                    }


//                    // Do stuff

//                    // With the points to add and the end and start indexes of the connection zone it is time to find the point of connection
//                    Point point_of_connection = first_points.at(first_end);  // by default end of connection


//                    // Draw line from the last new point to point of connection (used to check for possible different point of connection)
//                    Point last_new_point = points_to_add.back();

//                    if(common_indexes_in_second.back() == first_points.size()-1){ // If last conection point is the last overall point, the line actually goes from that to the begining, so the connection point should be later
//                        point_of_connection = common_elements.at(common_elements.size()-2);
//                    }

//                    Mat connection_line = Mat::zeros(first_mask.size(),CV_8U);
//                    line(connection_line,last_new_point,point_of_connection,WHITE,DRAW_WIDTH_1P);

//                    // Check for intersection with all other lines
//                    current_common_index = 0;
//                    vector<Point> possible_connection_points = {};
//                    vector<int> possible_connection_indexes = {};
//                    for(int  f = 0; f < first_points.size(); f++){
//                        if(f != common_indexes.at(current_common_index)){
//                            Mat current_line = Mat::zeros(first_mask.size(),CV_8U);
//                            int second_point_index = f+1;
//                            if(f != first_points.size()-1){
//                                line(current_line,first_points.at(f),first_points.at(f+1),WHITE,DRAW_WIDTH_1P);
//                            }
//                            else{
//                                line(current_line,first_points.at(f),first_points.at(0),WHITE,DRAW_WIDTH_1P);
//                                second_point_index = 0;
//                            }
//                            // Check for intersection that is not due to being connected to same point
//                            if(hasNonZero(current_line & connection_line) && point_of_connection != first_points.at(second_point_index)){
//                                possible_connection_points.push_back(first_points.at(f));
//                                possible_connection_indexes.push_back(f);
//                            }
//                        }
//                        else{
//                            if(current_common_index + 1 < first_points.size()){
//                                current_common_index++;
//                            }
//                        }
//                    }

//                    // Find ideal connection point
//                    int possible_connection_index = 0;
//                    bool reversed = false;
//                    if(possible_connection_points.size() == 1){
//                        point_of_connection = possible_connection_points.at(0);
//                        reveresed = true;
//                    }
//                    else if(possible_connection_points.size() > 1){
//                        reversed = true;
//                        // connect to shortest distance
//                        double min_dist = 0.0;
//                        for(int p = 0; p < possible_connection_points.size(); p++){
//                            double dist = calculations.calculate_euclidean_distance(last_new_point,possible_connection_points.at(p));
//                            if(p == 0 || dist < min_dist){
//                                min_dist = dist;
//                                point_of_connection = possible_connection_points.at(p);
//                                possible_connection_index = p;
//                            }
//                        }
//                    }

//                    // Make combination vector
//                    vector<Point> combined = {};

//                    // Add all new points
//                    for(int r = 0; r < remaining_points.size(); r++){
//                        combined.push_back(remaining_points.at(r));
//                    }

//                    // Add points in original polygon based on point of connection
//                    if(reversed == false){
//                        bool end_found = false;
//                        int start_index = common_indexes.back();
//                        if(common_indexes.back() == first_points.size()-1){ // If last conection point is the last overall point, the line actually goes from that to the begining, so the connection point should be later
//                            start_index = 0;
//                        }
//                        for(int f = start_index; f < first_points.size(); f++){
//                            combined.push_back(first_points.at(f));

//                            if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE) && f != start_index){
//                                end_found = true;
//                                break;
//                            }
//                        }

//                        // add beginning if entire part not found
//                        if(end_found == false){
//                            for(int f = 0; f < first_points.size(); f++){
//                                combined.push_back(first_points.at(f));
//                                if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE)){
//                                    end_found = true;
//                                    break;
//                                }
//                            }
//                        }
//                    }
//                    else{
//                        // Add points from reverse
//                        for(int f = poosible_connection_index; f >= 0; f--){
//                            combined.push_back(first_points.at(f));
//                            if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE)){
//                                break;
//                            }
//                        }
//                        // Include initial connection point
//                        Point final_point = common_elements.at(0);
//                        combined.push_back(final_point);
//                    }




//                    // Check if new points are on the same side of the intersection by splitting frame based on the two common elements
//                    Point start = Point(0,0);
//                    Point end = Point(0,0);
//                    Point p1 = common_elements.at(0);
//                    Point p2 = common_elements.back();
//                    double length = calculations.calculate_euclidean_distance(p1,p2);

//                    start.x = p2.x + (p2.x-p1.x)/length*2000;
//                    start.y = p2.y + (p2.y-p1.y)/length*2000;

//                    end.x = p1.x + (p1.x-p2.x)/length*2000;
//                    end.y = p1.y + (p1.y-p2.y)/length*2000;

//                    Mat split_frame = Mat::zeros(first_mask.size(), CV_8U);
//                    vector<vector<Point>> temp_contour = {};
//                    if(start.x < 0){
//                        temp_contour.push_back({start,end,Point(first_mask.cols-1,first_mask.rows-1),Point(0,first_mask.rows-1)});
//                    }
//                    else{
//                        temp_contour.push_back({start,end,Point(0,first_mask.rows-1),Point(first_mask.cols-1,first_mask.rows-1)});
//                    }
//                    drawContours(split_frame,temp_contour,0,WHITE,DRAW_WIDTH_INFILL);





//                    // Try normal connection from first connection point to second
//                    vector<Point> combined = {};
//                    // Go through all points in first polygon until intersection is met
//                    int index_of_return = 0;
//                    Point point_of_connection = Point(0,0);

//                    for(int f = 0; f < first_points.size(); f++){
//                        // Prepare point
//                        Point point = first_points.at(f);

//                        // add point
//                        combined.push_back(point);

//                        // Check if on intersection.
//                        if(intersection.at<uchar>(point) == uchar(WHITE)){
//                            // Continue adding if line is from end -> start since then end is the actually point of connection.
//                            bool start_found = true;
//                            if(f == 0){
//                                Point last_point = first_points.back();
//                                // If final point is also on line, that is the start and we should thus continue
//                                if(intersection.at<uchar>(last_point) == uchar(WHITE)){
//                                    start_found = false;
//                                }
//                            }
//                            //  If start found break and save next index and current point
//                            if(start_found == true){
//                                index_of_return = k+1;
//                                point_of_connection = point;
//                                break;
//                            }
//                        }
//                    }

//                }
//            }
//        }


//        // Create temp triangles vector, used for combining polygons
//        vector<triangle> temp_triangles = triangles;

//        // Go through all triangles and combine with neigbhors that makes it remain approximately convex
//        for(int i = 0; i < temp_triangles.size(); i++){
//            // Prepare index that keep track of triangles already combined successfully
//            vector<int> connected_indexes = {};
//            // Initialize data of first polygon
//            Mat first_mask = temp_triangles.at(i).mask;
//            vector<Point> first_points = temp_triangles.at(i).points;
//            // Go through triangles after the current one (since those before have already been evaluated)
//            for(int j = i+1; j < temp_triangles.size(); j++){
//                // Prepare mask of second polygon
//                Mat second_mask = temp_triangles.at(j).mask;
//                // Check if any part of triangles are intersecting
//                Mat intersection = first_mask & second_mask; // This is all that is needed since polygons share points and therefore do intersect when drawn
//                if(hasNonZero(intersection) == true){
//                    // Prepare second points
//                    vector<Point> second_points = temp_triangles.at(j).points;

//                    // test
//                    cout << "First points: " << endl;
//                    for(int k = 0; k < first_points.size(); k++){
//                        cout << first_points.at(k) << endl;
//                    }

//                    cout << "Second points: " << endl;
//                    for(int k = 0; k < second_points.size(); k++){
//                        cout << second_points.at(k) << endl;
//                    }

//                    // Initialise combined vector
//                    vector<Point> combined = {};
//                    // Go through all points in first polygon until intersection is met
//                    int index_of_return = 0;
//                    Point point_of_connection = Point(0,0);
//                    bool connectable = true;
//                    for(int k = 0; k < first_points.size(); k++){
//                        // Prepare point
//                        Point point = first_points.at(k);
//                        // add point
//                        combined.push_back(point);
//                        // Check if on intersection.
//                        if(intersection.at<uchar>(point) == uchar(WHITE)){
//                            // Ensure that previous point is not on line if index is 0
//                            bool start_found = true;
//                            if(k == 0){
//                                Point last_point = first_points.back();
//                                // If final point is also on line, that is the start and we should thus continue
//                                if(intersection.at<uchar>(last_point) == uchar(WHITE)){
//                                    start_found = false;
//                                }
//                            }
//                            //  If start found break and save next index and current point
//                            if(start_found == true){
//                                index_of_return = k+1;
//                                // If there is no other connection return the result would never be convex
//                                int temp_index = k+1;
//                                if(temp_index >= first_points.size()){
//                                    temp_index = 0;
//                                }
//                                Point next_point = first_points.at(temp_index);
//                                if(intersection.at<uchar>(next_point) != uchar(WHITE)){
//                                    connectable = false;
//                                }
//                                point_of_connection = point;
//                                break;
//                            }
//                        }
//                    }
//                    // continue if not connectable
//                    if(connectable == false){
//                        continue;
//                    }
//                    // Find point in second polygon from where to begin connection (point that is the same as the point of connection)
//                    bool connection_found = false;
//                    bool clockwise = true; // standard clockwise
//                    int start_index = 0;
//                    Point start_point = Point(0,0);
//                    for(int k = 0; k < second_points.size(); k++){
//                        Point point = second_points.at(k);
//                        // Check if point is at connection
//                        if(point == point_of_connection){
//                            connection_found = true;
//                            // Get nex and previous points
//                            int previous_index = k-1;
//                            int next_index = k+1;
//                            if(previous_index < 0){
//                                previous_index = second_points.size()-1;
//                            }
//                            if(next_index >= second_points.size()){
//                                next_index = 0;
//                            }
//                            Point previous_point = second_points.at(previous_index);
//                            Point next_point = second_points.at(next_index);
//                            // Find direction by checking if previous or next point is on intersection
//                            if(intersection.at<uchar>(previous_point) == uchar(BLACK) && intersection.at<uchar>(next_point) == uchar(WHITE)){
//                                clockwise = false;
//                                start_index = previous_index;
//                                start_point = previous_point;
//                            }
//                            else{
//                                start_index = next_index;
//                                start_point = next_point;
//                            }
//                            break;
//                        }
//                    }
//                    // continue if no connection was found or next point is on intersection. Otherwise add second polygons based on direction
//                    if(connection_found == false || intersection.at<uchar>(start_point) == uchar(WHITE)){
//                        continue;
//                    }
//                    else if(clockwise == true){
//                        // add points in clockwise order until intersection is met
//                        int zero_addon = 0;
//                        for(int k = 0; k < second_points.size(); k++){
//                            // Current index
//                            int current_index = start_index+k;
//                            if(current_index >= second_points.size()){
//                                current_index = 0+zero_addon;
//                                zero_addon++;
//                            }
//                            // Current point
//                            Point current_point = second_points.at(current_index);
//                            // Check if point is on intersection if so stop
//                            if(intersection.at<uchar>(current_point) == uchar(WHITE)){
//                                break;
//                            }
//                            else{
//                                combined.push_back(current_point);
//                            }
//                        }
//                    }
//                    else{
//                        // add points in counter clockwise order until intersection is met
//                        bool end_found = false;
//                        for(int k = start_index; k >= 0; k--){
//                            // Current point
//                            Point current_point = second_points.at(k);
//                            // Check if point is on intersection if so stop
//                            if(intersection.at<uchar>(current_point) == uchar(WHITE)){
//                                end_found = true;
//                                break;
//                            }
//                            else{
//                                combined.push_back(current_point);
//                            }
//                        }
//                        // Add remaining points if end not found yet
//                        if(end_found == false){
//                            for(int k = 0; k < start_index; k++){
//                                // Current point
//                                Point current_point = second_points.at(k);
//                                // Check if point is on intersection if so stop
//                                if(intersection.at<uchar>(current_point) == uchar(WHITE)){
//                                    break;
//                                }
//                                else{
//                                    combined.push_back(current_point);
//                                }
//                            }
//                        }
//                    }
//                    // Add remaining of initial polygon
//                    for(int k = index_of_return; k < first_points.size(); k++){
//                        // Prepare point
//                        Point point = first_points.at(k);
//                        // add point
//                        combined.push_back(point);
//                    }

//                    // test
//                    cout << "Combined points: " << endl;
//                    for(int k = 0; k < combined.size(); k++){
//                        cout << combined.at(k) << endl;
//                    }

//                    // Check for convexity
//                    vector<int> convex_hull_indexes;
//                    convexHull(combined,convex_hull_indexes);

//                    vector<Vec4i> defects;
//                    convexityDefects(combined,convex_hull_indexes,defects);

//                    // Check individual depths
//                    bool approx_convex = true;

//                    for(int defect_index = 0; defect_index < defects.size(); defect_index++){
//                        float depth = float(defects.at(defect_index)[3])/256.0;
//                        if(depth > 2.0){ // If big depth then dont accept connection
//                            approx_convex = false;
//                            continue;
//                        }
//                    }

//                    if(approx_convex == true){
//                        // Update triangle
//                        temp_triangles.at(i).mask = first_mask | second_mask;
//                        temp_triangles.at(i).points = combined;
//                        first_points = combined;
//                        first_mask = temp_triangles.at(i).mask;
//                        connected_indexes.push_back(j);
//                    }
//                }
//            }
//            // Removed conencted polygons
//            for(int j = connected_indexes.size()-1; j >= 0; j--){
//                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
//            }

//            // Add remaining triangles to output
//            combined_triangles.push_back(temp_triangles.at(i));


//            // Test visualization
//            vector<vector<Point>> test_vec = {{temp_triangles.at(i).points}};
//            Mat point_draw = Mat::zeros(temp_triangles.at(i).mask.size(), CV_8U);
//            drawContours(point_draw,test_vec,0,WHITE,DRAW_WIDTH_INFILL);
//            imshow("new mask", temp_triangles.at(i).mask);
//            imshow("point contour",point_draw);
//            waitKey(0);
//        }



//        vector<triangle> temp_triangles = triangles;
//        for(int i = 0; i < temp_triangles.size();i++){
//           vector<int> connected_indexes = {};

//            for(int j = i+1; j < temp_triangles.size(); j++){

//                // Check if two triangles are touching
//                Mat intersection = temp_triangles.at(i).mask & temp_triangles.at(j).mask;

//                if(hasNonZero(intersection) == true){
//                    // Get points of two triangles
//                    vector<Point> first_triangle = temp_triangles.at(i).points;
//                    vector<Point> second_triangle = temp_triangles.at(j).points;

//                    cout << "First triangle: " << endl;
//                    for(int k = 0; k < first_triangle.size(); k++){
//                        cout << first_triangle.at(k) << endl;
//                    }

//                    cout << "Second triangle: " << endl;
//                    for(int k = 0; k < second_triangle.size(); k++){
//                        cout << second_triangle.at(k) << endl;
//                    }

//                    // Remove non unique points from second triangle
//                    vector<Point> unique_points = {};
//                    for(int l = 0; l < second_triangle.size(); l++){
//                        bool unique = true;
//                        for(int k = 0; k < first_triangle.size(); k++){
//                            if(second_triangle.at(l) == first_triangle.at(k)){
//                                unique = false;
//                                break;
//                            }
//                        }
//                        if(unique == true){
//                            unique_points.push_back(second_triangle.at(l));
//                        }
//                    }

//                    // Find edge of interesection
//                    int insert_index = 0;
//                    int second_insert_index = 0;
//                    for(int k = 0; k < first_triangle.size(); k++){

//                        // Define last indexes
//                        int second_index = k+1;
//                        if(k == first_triangle.size()-1){
//                            second_index = 0;
//                        }

//                        // define line segment
//                        Point p1 = first_triangle.at(k);
//                        Point p2 = first_triangle.at(second_index);

//                        // go through lines in second polygon
//                        bool intersect_found = false;
//                        for(int l = 0; l < second_triangle.size(); l++){
//                            int second_l = l+1;
//                            if(l == second_triangle.size()-1){
//                                second_l = 0;
//                            }
//                            Point q1 = second_triangle.at(l);
//                            Point q2 = second_triangle.at(second_l);

//                            // Check for intersection
//                            bool intersects = calculations.do_intersect(p1,p2,q1,q2);

//                            if(intersects == true){
//                                // Ensure intersection is includes at least one unique value
//                                if((q1 != p1 && q1 != p2) || (q2 != p1 && q2 != p2)){
//                                    // Set insert index in between the two points
//                                    if(second_index == k+1){
//                                        insert_index = k;
//                                    }
//                                    else{
//                                        insert_index = 0;
//                                    }

//                                    // choose connection point in second triangle based on it being unique or the line that intersects least with the original mask
//                                    if((q1 != p1 && q1 != p2) && (q2 == p1 || q2 == p2)){
//                                        second_insert_index = l;
//                                    }
//                                    else if((q2 != p1 && q2 != p2) && (q1 == p1 || q1 == p2)){
//                                        second_insert_index = second_l;
//                                    }
//                                    else{
//                                        Mat line1 = Mat::zeros(triangles.at(0).mask.size(), CV_8U);
//                                        Mat line2 = Mat::zeros(triangles.at(0).mask.size(), CV_8U);

//                                        line(line1,p1,q1,WHITE);
//                                        line(line2,p1,q2,WHITE);

//                                        Mat intersect1 = line1 & temp_triangles.at(i).mask;
//                                        Mat intersect2 = line2 & temp_triangles.at(i).mask;

//                                        if(countNonZero(intersect1) <= countNonZero(intersect2)){
//                                            second_insert_index = l;
//                                        }
//                                        else{
//                                            second_insert_index = second_l;
//                                        }
//                                    }

//                                    intersect_found = true;
//                                    break;
//                                }

//                            }
//                        }
//                        if(intersect_found == true){
//                            break;
//                        }
//                    }

//                    // Combine points
//                    vector<Point> combined_points = {};

//                    cout << "Insert at: " << first_triangle.at(insert_index);
//                    cout << "Insert from: " << second_triangle.at(second_insert_index);

//                    cout << "Combined polygon: " << endl;
//                    for(int k = 0; k <= insert_index; k++){
//                        combined_points.push_back(first_triangle.at(k));
//                        cout << first_triangle.at(k) << endl;
//                    }
//                    for(int k = second_insert_index; k < second_triangle.size(); k++){
//                        if(count(unique_points.begin(),unique_points.end(),second_triangle.at(k)) > 0){
//                            combined_points.push_back(second_triangle.at(k));
//                            cout << second_triangle.at(k) << endl;
//                        }
//                    }
//                    for(int k = 0; k < second_insert_index; k++){
//                        if(count(unique_points.begin(),unique_points.end(),second_triangle.at(k)) > 0){
//                            combined_points.push_back(second_triangle.at(k));
//                            cout << second_triangle.at(k) << endl;
//                        }
//                    }
//                    for(int k = insert_index+1; k < first_triangle.size(); k++){
//                        combined_points.push_back(first_triangle.at(k));
//                        cout << first_triangle.at(k) << endl;
//                    }

//                    cout << combined_points.size() << " = " << first_triangle.size() << " + " << unique_points.size() << endl;
//                    cout << "second and first index are wrong" << endl;
//                    // Check for convexity
//                    vector<int> convex_hull_indexes;
//                    convexHull(combined_points,convex_hull_indexes);

//                    vector<Vec4i> defects;
//                    convexityDefects(combined_points,convex_hull_indexes,defects);

//                    // Check individual depths
//                    bool approx_convex = true;

//                    for(int defect_index = 0; defect_index < defects.size(); defect_index++){
//                        float depth = float(defects.at(defect_index)[3])/256.0;
//                        if(depth > 2.0){ // If big depth then dont accept connection
//                            approx_convex = false;
//                            break;
//                        }
//                    }

//                    if(approx_convex == true){
//                    // Update triangle
//                    temp_triangles.at(i).mask = temp_triangles.at(i).mask | temp_triangles.at(j).mask;
//                    temp_triangles.at(i).points = combined_points;
//                    connected_indexes.push_back(j);
//                    }
//                }
//            }
//            // Remove triangles that have been connected
//            for(int j = connected_indexes.size()-1; j >= 0; j--){
//                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
//            }

//            // Add remaining triangle
//            combined_triangles.push_back(temp_triangles.at(i));
//            vector<vector<Point>> test_vec = {{temp_triangles.at(i).points}};
//            Mat point_draw = Mat::zeros(temp_triangles.at(i).mask.size(), CV_8U);
//            drawContours(point_draw,test_vec,0,WHITE,DRAW_WIDTH_INFILL);
//            imshow("new mask", temp_triangles.at(i).mask);
//            imshow("point contour",point_draw);
//            waitKey(0);
//        }



//        // Go through all triangles and combine with neigbhors that makes it remain approximately convex
//        vector<triangle> temp_triangles = triangles;

//        for(int i = 0; i < temp_triangles.size();i++){
//            vector<int> connected_indexes = {};

//            for(int j = i+1; j < temp_triangles.size(); j++){

//                // Get points of two triangles
//                vector<Point> first_triangle = temp_triangles.at(i).points;
//                vector<Point> second_triangle = temp_triangles.at(j).points;

//                // Ensure that triangles are not connected at a single point
//                int same_count = 0;
//                int first_same_index = -1;
//                int last_same_index = -1;

//                for(int k = 0; k < first_triangle.size(); k++){
//                    for(int l = 0; l < second_triangle.size(); l++){
//                        if(first_triangle.at(k) == second_triangle.at(l)){
//                            same_count++;
//                            if(first_same_index == -1){
//                                first_same_index = k;
//                            }
//                            last_same_index = k;
//                        }
//                    }
//                }

//                // Check if triangles are next to eachother
//                Mat dilated = temp_triangles.at(i).mask.clone();
//                dilate(dilated,dilated,border_kernel);
//                Mat intersect = dilated & temp_triangles.at(j).mask;

//                if(countNonZero(intersect) > 1 && same_count != 1){ // single pixel connections result in intersections

//                    // Get unique points in second triangle
//                    vector<Point> unique_points = {};
//                    for(int l = 0; l < second_triangle.size(); l++){
//                        bool unique = true;
//                        for(int k = 0; k < first_triangle.size(); k++){
//                            if(second_triangle.at(l) == first_triangle.at(k)){
//                                unique = false;
//                                break;
//                            }
//                        }
//                        if(unique == true){
//                            unique_points.push_back(second_triangle.at(l));
//                        }

//                    // Draw all original lines
//                    Mat line_drawing = Mat::zeros(triangles.at(0).mask.size(), CV_8U);
//                    for(int k = 0; k < first_triangle.size(); k++){

//                    }

////                    // If two points intersect. Add new point inbetween those
////                    vector<Point> new_points = {};

////                    if(same_count == 2){
////                        // add point in between the two identical points
////                        if(first_same_index == 0 && last_same_index == first_triangle.size()-1){
////                            for(int l = 0; l < unique_points.size(); l++){
////                                new_points.push_back(unique_points.at(l));
////                            }
////                            new_points.insert(new_points.end(),first_triangle.begin(),first_triangle.end());
////                        }
////                        else{
////                            for(int k = 0; k < first_same_index; k++){
////                                new_points.push_back(first_triangle.at(k));
////                            }
////                            new_points.insert(new_points.end(),unique_points.begin(), unique_points.end());
////                            for(int k = last_same_index; k < first_triangle.size(); k++){
////                                new_points.push_back(first_triangle.at(k));
////                            }
////                        }
////                    }
////                    else{
////                        // Find closest combination between two triangles
////                        double min_dist = 0.0;
////                        int best_first_index = 0;
////                        int best_second_index = 0;
////                        for(int k = 0; k < first_triangle.size(); k++){
////                            for(int l = 0; l < unique_points.size(); l++){
////                                // calcualte distance
////                                double dist = calculations.calculate_euclidean_distance(first_triangle.at(k),second_triangle.at(l));

////                                if(dist < min_dist || (k == 0 && l == 0)){
////                                    min_dist = dist;
////                                    best_first_index = k;
////                                    best_second_index = l;
////                                }
////                            }
////                        }

//                        // Combine triangles at this connection;
////                        for(int k = 0; k < first_triangle.size(); k++){
////                            new_points.push_back(first_triangle.at(k));

////                            if(k == best_first_index){

////                                for(int l = best_second_index; l < second_triangle.size(); l++){
////                                    new_points.push_back(second_triangle.at(l));
////                                }
////                                for(int l = 0; l < best_second_index; l++){
////                                    new_points.push_back(second_triangle.at(l));
////                                }
////                            }
////                        }
////                    }


//                    cout << "First triangle" << endl;
//                    for(int test = 0; test < first_triangle.size(); test++){
//                        cout << first_triangle.at(test) << endl;
//                    }
//                    cout << "Second triangle" << endl;
//                    for(int test = 0; test < second_triangle.size(); test++){
//                        cout << second_triangle.at(test) << endl;
//                    }
//                    cout << "Combined triangle" << endl;
//                    for(int test = 0; test < new_points.size(); test++){
//                        cout << new_points.at(test) << endl;
//                    }

//                    // Create new mask
//                    Mat combined_mask = Mat::zeros(triangles.at(0).mask.size(),CV_8U);
//                    vector<vector<Point>> temp_points = {{new_points}};
//                    drawContours(combined_mask,temp_points,0,WHITE,DRAW_WIDTH_INFILL);

//                    // Check for convexity
//                    vector<int> convex_hull_indexes;
//                    convexHull(new_points,convex_hull_indexes);

//                    vector<Vec4i> defects;
//                    convexityDefects(new_points,convex_hull_indexes,defects);

//                    // Check individual depths
//                    bool approx_convex = true;

//                    for(int defect_index = 0; defect_index < defects.size(); defect_index++){
//                        float depth = float(defects.at(defect_index)[3])/256.0;
//                        if(depth > 2.0){ // If big depth then dont accept connection
//                            approx_convex = false;
//                            break;
//                        }
//                    }

//                    if(approx_convex == true){
//                        temp_triangles.at(i).mask = combined_mask;
//                        temp_triangles.at(i).points = new_points;
//                        connected_indexes.push_back(j);
//                    }


//                }
//            }

//            // Remove triangles that have been connected
//            for(int j = connected_indexes.size()-1; j >= 0; j--){
//                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
//            }

//            // Add remaining triangle
//            combined_triangles.push_back(temp_triangles.at(i));
//        }




////                if(i != j){


////                    // Get smallest edge
////                    double smallest_edge = 0;

////                    vector<Point> first_triangle =  temp_triangles.at(i).points;
////                    vector<Point> second_triangle = temp_triangles.at(j).points;

////                    vector<vector<Point>> point_combos = {{first_triangle.at(0),first_triangle.at(1)},{first_triangle.at(1),first_triangle.at(2)},{first_triangle.at(2),first_triangle.at(0)},{second_triangle.at(0),second_triangle.at(1)},{second_triangle.at(1),second_triangle.at(2)},{second_triangle.at(2),second_triangle.at(0)}};

////                    for(int k = 0; k < point_combos.size(); k++){
////                        double edge = fabs(calculations.calculate_euclidean_distance(point_combos.at(k).at(0),point_combos.at(k).at(1)));
////                        if(edge < smallest_edge || k == 0){
////                            smallest_edge = edge;
////                        }
////                    }

////                    // Check if connected
////                    Mat dilated = temp_triangles.at(i).mask.clone();
////                    dilate(dilated,dilated,border_kernel);
////                    Mat intersect = dilated & temp_triangles.at(j).mask;

////                    if(countNonZero(intersect) >= cvRound(smallest_edge)){ // bigger than smallest size since one pixel connection is bad (intersects with itself)
////                        // find contour of combined
////                        Mat combined = temp_triangles.at(i).mask | temp_triangles.at(j).mask;
////                        vector<vector<Point>> contours;
////                        findContours(combined,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

////                        cout << "Initial points: " << endl;
////                        for(int test = 0; test < 3; test++){
////                            cout << first_triangle.at(test) << endl;
////                        }
////                        for(int test = 0; test < 3; test++){
////                            cout << second_triangle.at(test) << endl;
////                        }

////                        cout << "New points: " << endl;
////                        for(int test = 0; test < contours.at(0).size(); test++){
////                            cout << contours.at(0).at(test) << endl;
////                        }


////                        // Check for convexity
////                        vector<int> convex_hull_indexes;
////                        convexHull(contours.at(0),convex_hull_indexes);

////                        vector<Vec4i> defects;
////                        convexityDefects(contours.at(0),convex_hull_indexes,defects);

////                        // Check individual depths
////                        bool approx_convex = true;


//////                        if(i >= 2){
//////                            Mat test_draw = Mat::zeros(triangles.at(0).size(), CV_8U);
//////                            cout << "new: " << endl;
//////                            for(int test = 0; test < contours.at(0).size()-1; test++){
//////                                line(test_draw,contours.at(0).at(test),contours.at(0).at(test+1),WHITE,1);
//////                                cout << contours.at(0).at(test) << endl;
//////                            }
//////                            cout << contours.at(0).at(contours.at(0).size()-1) << endl;
//////                            imshow("draw",test_draw);
//////                            waitKey(0);
//////                        }

////                        for(int defect_index = 0; defect_index < defects.size(); defect_index++){
////                            float depth = float(defects.at(defect_index)[3])/256.0;
////                            if(depth > 2){
////                                approx_convex = false;
////                                break;
////                            }
////                        }

////                        if(approx_convex == true){
////                            temp_triangles.at(i).mask = combined;
////                            connected_indexes.push_back(j);
////                        }
////                    }
////                }
////            }

////            // Remove triangles that have been connected
////            for(int j = connected_indexes.size()-1; j >= 0; j--){
////                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
////            }

////            // Add remaining triangle
////            combined_triangles.push_back(temp_triangles.at(i));
////        }



////        // Go through all triangles
////        for(int i = 0; i < triangles.size(); i++){
////            // Find neighbors and add if still convex until nothing is added
////            bool change = true;
////            cout << "New start" << endl;
////            while(change == true){
////                change = false;
////                vector<int> connected_indexes = {};
////                for(int j = 0; j < triangles.size(); j++){
////                    if(i != j){
////                        // Check if connected
////                        Mat combined = triangles.at(i) | triangles.at(j);
////                        vector<vector<Point>> contours;
////                        findContours(combined,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

////                        if(contours.size() == 1){ // If only one contour, they must be connected
////                            // Check for convexity
////                            cout << "is connected" << endl;

////                            // Get convex hull
////                            vector<int> convex_hull_indexes;
////                            convexHull(contours.at(0),convex_hull_indexes);

////                            // Find defects
////                            vector<Vec4i> defects;
////                            convexityDefects(contours.at(0),convex_hull_indexes,defects);

////                            // Check depth individual depths
////                            bool approx_convex = true;
////                            for(int defect_index = 0; defect_index < defects.size(); defect_index++){
////                                float depth = float(defects.at(defect_index)[3])/256.0;
////                                if(depth > 2){
////                                    approx_convex = false;
////                                    break;
////                                }
////                            }

////                            if(approx_convex == true){
////                                cout << "is convex" << endl;
////                                triangles.at(i) = combined;
////                                connected_indexes.push_back(j);
////                                change = true;
////                            }
////                        }
////                        imshow("combined",combined);
////                        waitKey(0);
////                    }
////                }
////                cout << "am out" << endl;
////                // Removed used elements
////                for(int j = connected_indexes.size()-1; j >= 0; j--){
////                    triangles.erase(triangles.begin()+connected_indexes.at(j));
////                }
////            }
////            combined_triangles.push_back(triangles.at(i));
////        }

//                        // Find common elements
//                        int common_count = 0;
//                        vector<Point> unique_points = {};
//                        for(int j = 0; j < uncombined_polygons.at(i).original_points.size(); j++){
//                            bool common_found = false;
//                            for(int k = 0; k < unused_polygons.at(index).original_points.size(); k++){
//                                if(unused_polygons.at(index).original_points.at(k) == uncombined_polygons.at(i).original_points.at(j)){
//                                    common_found = true;
//                                }
//                            }
//                            if(common_found == true){
//                                common_count++;
//                            }
//                            else{
//                                unique_points.push_back(uncombined_polygons.at(i).points.at(j));
//                            }
//                        }

//                        // Continue if less than two common points
//                        if(common_count < 2){
//                            continue;
//                        }

//                        // Close mask to avoid intersections
//                        morphologyEx(combined,combined,MORPH_CLOSE,border_kernel);

//                        cout << "New combination: " << endl;
//                        for(int o = 0; o < points.size(); o++){
//                            cout << points.at(o) << endl;
//                        }

//                        // Ensure that polygon does not self intersect
//                        bool intersects = calculations.do_intersect(points);
//                        //bool intersects = check_polygon_for_holes(combined);

//                        if(intersects == true){
//                            continue;
//                        }

//waitKey(0);
//                        cout << "Intersection count: " << countNonZero(intersection) << endl;
//                        imshow("combi",combined);
//                        waitKey(0);

//                cout << "good obstacle dies here" << endl;
//                // New idea: Keep the obstacle containing most matches if similar
//                for(int i = 0; i < temp_obstacles.size(); i++){
////                    imshow("initial mask",temp_obstacles.at(i).mask);
//                    vector<int> to_be_removed = {};
//                    for(int j = i+1; j < temp_obstacles.size(); j++){
//                        Mat intersection = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
//                        int intersection_count = countNonZero(intersection);

//                        // If intersection is 75% or more of either obstacle and similar angle, they are similar
//                        int first_count = countNonZero(temp_obstacles.at(i).mask);
//                        int second_count =  countNonZero(temp_obstacles.at(j).mask);

//                        if((intersection_count >= (int)first_count*0.75 || intersection_count >= (int)second_count*0.75) && is_angle_vertical(temp_obstacles.at(i).original_angle) == is_angle_vertical(temp_obstacles.at(i).original_angle)){
//                            to_be_removed.push_back(j);
//                            // Update initial obstacle based on which has the most matches with disparity map
//                            if(second_count > first_count){
////                                if(i == 0){
////                                    imshow("killed",temp_obstacles.at(i).mask);
////                                    imshow("killer",temp_obstacles.at(j).mask);
////                                    waitKey(0);
////                                }
//                                temp_obstacles.at(i).mask = temp_obstacles.at(j).mask;
//                            }
////                            else{
////                                imshow("killed",temp_obstacles.at(i).mask);
////                                imshow("killer",temp_obstacles.at(j).mask);
////                            }
////                            waitKey(0);
//                        }
//                    }
////                    imshow("final mask",temp_obstacles.at(i).mask);
////                    waitKey(0);
//                    // removed used indexes
//                    if(to_be_removed.size() > 0){
//                        for(int j = to_be_removed.size()-1; j >= 0; j--){
//                            temp_obstacles.erase(temp_obstacles.begin()+to_be_removed.at(j));
//                        }
//                    }
//                    // add current obstacle to accepted obstacles
//                    temp_obstacles.at(i).contour = get_biggest_contour(temp_obstacles.at(i).mask);

//                    vector<vector<Point>> temp_vec = {temp_obstacles.at(i).contour};

//                    Mat biggest_contour_mask = Mat::zeros(mask.size(),CV_8U);

//                    drawContours(biggest_contour_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);

//                    temp_obstacles.at(i).mask = biggest_contour_mask;

//                    accepted_obstacles.push_back(temp_obstacles.at(i));
//                }

//                cout << "Obstacles that survived similarity test: " << accepted_obstacles.size() << endl;;
//                for(int k = 0; k < accepted_obstacles.size(); k++){
//                    string name = to_string(k);
//                    imshow(name,accepted_obstacles.at(k).mask);
//                    waitKey(0);
//                }

                // Play with similarity between obstacles (keep only similar elements) (wird stuff in here maybe just remove)
//                for(int i = 0; i < temp_obstacles.size(); i++){

//                    Mat new_version = temp_obstacles.at(i).mask;
//                    vector<int> remove_these = {};

//                    for(int j = i+1; j < temp_obstacles.size(); j++){
//                        Mat intersection = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
//                        int intersection_count = countNonZero(intersection);
//                        if(intersection_count >= (int)countNonZero(temp_obstacles.at(i).mask)*0.95){
//                            new_version = new_version & temp_obstacles.at(j).mask;
//                            remove_these.push_back(j);
//                        }
//                    }
//                    if(remove_these.size() > 0){
//                        for(int j = remove_these.size()-1; j >= 0; j--){
//                            temp_obstacles.erase(temp_obstacles.begin()+remove_these.at(j));
//                        }
//                    }
//                    // Keep if not almost completely within all other obstacles
//                    temp_obstacles.at(i).contour = get_biggest_contour(new_version);
//                    vector<vector<Point>> temp_vec = {temp_obstacles.at(i).contour};
//                    Mat new_version_contour_mask = Mat::zeros(new_version.size(),CV_8U);
//                    drawContours(new_version_contour_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
//                    temp_obstacles.at(i).mask = new_version_contour_mask;
//                    accepted_obstacles.push_back(temp_obstacles.at(i));
//                }

//                cout << "obstacles remaining after similarity test: " << accepted_obstacles.size() << endl;

                // Maybe if two are similar but not very similar and have similar angles, use average angle to create new line and use that to create cut (seem to make things worse)
//                for(int i = 0; i < accepted_obstacles.size(); i++){
//                    vector<int> to_be_removed = {};
//                    for(int j = i+1; j < accepted_obstacles.size(); j++){
//                        Mat intersection = accepted_obstacles.at(i).mask & accepted_obstacles.at(j).mask;
//                        int intersection_count = countNonZero(intersection);
//                        if(intersection_count >= countNonZero(accepted_obstacles.at(i).mask)*0.75){

//                            vector<Point> contour, second_contour;
//                            findNonZero(accepted_obstacles.at(i).mask,contour);
//                            findNonZero(accepted_obstacles.at(j).mask,second_contour);

//                            // Get best fit lines and angles
//                            Vec4f fitted_line, second_fitted_line;
//                            fitLine(contour,fitted_line, DIST_L2, 0, 0.1, 0.1);
//                            fitLine(second_contour,second_fitted_line, DIST_L2, 0, 0.1, 0.1);

//                            int biggest_distance = int(sqrt(accepted_obstacles.at(i).mask.cols*accepted_obstacles.at(i).mask.cols + accepted_obstacles.at(i).mask.rows * accepted_obstacles.at(i).mask.rows));

//                            Point start = Point(fitted_line[2],fitted_line[3]);
//                            Point end = Point(fitted_line[2]+fitted_line[0]*biggest_distance,fitted_line[3]+fitted_line[1]*biggest_distance);

//                            Point second_start = Point(second_fitted_line[2],second_fitted_line[3]);
//                            Point second_end =  Point(second_fitted_line[2]+second_fitted_line[0]*biggest_distance,second_fitted_line[3]+second_fitted_line[1]*biggest_distance);

//                            Mat tempi = Mat::zeros(accepted_obstacles.at(i).mask.size(),CV_8U);
//                            line(tempi,start,end,WHITE);
//                            line(tempi,second_start,second_end,WHITE);
//                            imshow("the lines",tempi);
//                            waitKey(0);


//                            double angle = calculations.calculate_angle(start,end);
//                            double second_angle = calculations.calculate_angle(second_start,second_end);
//                            double angle = accepted_obstacles.at(i).original_angle;
//                            double second_angle = accepted_obstacles.at(j).original_angle;


//                            cout << angle*180/M_PI << " | " << second_angle*180/M_PI << endl;
//                            double diff = abs(angle*180/M_PI - abs(second_angle*180/M_PI));
//                            cout << "the diff: " << diff << endl;
//                            if(diff < 10.0){
//                                double average_angle = (angle+second_angle)/2.0;
//                                cout << "average angle: " << average_angle*180/M_PI << endl;

//                                Point mask_center = get_contour_center(accepted_obstacles.at(i).mask);

//                                int range = max(accepted_obstacles.at(i).mask.cols,accepted_obstacles.at(i).mask.rows);

//                                Vec4i new_line;
//                                double a = sin(angle);
//                                double b = cos(angle);

//                                new_line[0] = (int)cvRound(mask_center.x - range*b);
//                                new_line[1] = (int)cvRound(mask_center.y - range*a);
//                                new_line[2] = (int)cvRound(mask_center.x + range*b);
//                                new_line[3] = (int)cvRound(mask_center.y + range*a);

//                                int direction;
//                                double dist_to_90 = abs(90*M_PI/180-abs(angle));
//                                if(abs(angle*180/M_PI) > dist_to_90*180/M_PI){
//                                    direction = DIRECTION_RIGHT;
//                                }
//                                else{
//                                    direction = DIRECTION_UP;
//                                }

//                                vector<Vec4i> borders = get_best_fit_borders(direction,new_line,mask);

//                                Mat best_fit_mask = Mat::zeros(accepted_obstacles.at(i).mask.size(),CV_8U);

//                                vector<Point> points = {Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]), Point(borders.at(0)[2],borders.at(0)[3])};
//                                vector<vector<Point>> temp = {points};
//                                drawContours(best_fit_mask,temp,0,WHITE,DRAW_WIDTH_INFILL);

//                                Mat final_mask = best_fit_mask & mask;
//                                accepted_obstacles.at(i).mask = final_mask;
//                                accepted_obstacles.at(i).contour = get_biggest_contour(final_mask);

//                                final_mask = Mat::zeros(best_fit_mask.size(),CV_8U);
//                                vector<vector<Point>> temp_vec = {accepted_obstacles.at(i).contour};

//                                drawContours(final_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
//                                accepted_obstacles.at(i).mask = final_mask;

//                                to_be_removed.push_back(j);
//                            }
//                            else{
//                                // keep the one that contains more mask values (seen as better fit)
//                                if(countNonZero(accepted_obstacles.at(i).mask) <  countNonZero(accepted_obstacles.at(j).mask)){
//                                    accepted_obstacles.at(i) = accepted_obstacles.at(j);
//                                }
//                                to_be_removed.push_back(j);
//                                cout << countNonZero(accepted_obstacles.at(i).mask) << " | " << countNonZero(accepted_obstacles.at(j).mask) << endl;
//                            }
//                        }
//                    }
//                    if(to_be_removed.size() > 0){
//                        for(int j = to_be_removed.size()-1; j >= 0; j--){
//                            accepted_obstacles.erase(accepted_obstacles.begin()+to_be_removed.at(j));
//                        }
//                    }
//                }
//                cout << "obstacles remaining after angle test: " << accepted_obstacles.size() << endl;
//                for(int be = 0; be < accepted_obstacles.size(); be++){
//                    string name = to_string(be);
//                    imshow(name,accepted_obstacles.at(be).mask);
//                    waitKey(0);
//                }
//                waitKey(0);
