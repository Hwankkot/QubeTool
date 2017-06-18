
CManagement* Get_Management(void)
{
	return CManagement::GetInstance();
}

CLightMgr* Get_LightMgr(void)
{
	return CLightMgr::GetInstance();
}

CFrustum* Get_Frustum(void)
{
	return CFrustum::GetInstance();
}

CQuadTree* Get_Quadtree(void)
{
	return CQuadTree::GetInstance();
}

CNavMgr* Get_NavMgr(void)
{
	return CNavMgr::GetInstance();
}